#include "gui/VisualizadorAutomataWidget.h"

#include "automata/DFA.h"
#include "automata/DFAUnion.h"
#include "estructuras/NodoEstado.h"
#include "estructuras/NodoEstadoCompuesto.h"
#include "estructuras/NodoSimbolo.h"
#include "estructuras/NodoTransicion.h"
#include "estructuras/NodoTransicionCompuesta.h"

#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsPathItem>
#include <QGraphicsPolygonItem>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QPainterPath>
#include <QPen>
#include <QPolygonF>
#include <QPushButton>
#include <QVBoxLayout>
#include <cmath>

namespace {
const double PI = 3.14159265358979323846;
const double NODE_DIAMETER = 70.0;

QString estadoCompuesto(const std::string& primero, const std::string& segundo) {
    return "(" + QString::fromStdString(primero) + "," + QString::fromStdString(segundo) + ")";
}

QPointF puntoBorde(double x, double y, double diametro, double angulo) {
    const double radio = diametro / 2.0;
    return QPointF(x + radio * std::cos(angulo), y + radio * std::sin(angulo));
}

bool mismaPareja(const std::string& o1, const std::string& d1,
                 const std::string& o2, const std::string& d2) {
    return (o1 == o2 && d1 == d2) || (o1 == d2 && d1 == o2);
}

bool transicionMenorEnPareja(const std::string& o1, const std::string& s1, const std::string& d1,
                             const std::string& o2, const std::string& s2, const std::string& d2) {
    if (o1 != o2) return o1 < o2;
    if (s1 != s2) return s1 < s2;
    return d1 < d2;
}

bool mismaParejaCompuesta(const std::string& o11, const std::string& o12,
                          const std::string& d11, const std::string& d12,
                          const std::string& o21, const std::string& o22,
                          const std::string& d21, const std::string& d22) {
    const bool iguales = o11 == o21 && o12 == o22 && d11 == d21 && d12 == d22;
    const bool inversas = o11 == d21 && o12 == d22 && d11 == o21 && d12 == o22;
    return iguales || inversas;
}

bool transicionMenorEnParejaCompuesta(const std::string& o11, const std::string& o12,
                                      const std::string& s1, const std::string& d11,
                                      const std::string& d12, const std::string& o21,
                                      const std::string& o22, const std::string& s2,
                                      const std::string& d21, const std::string& d22) {
    if (o11 != o21) return o11 < o21;
    if (o12 != o22) return o12 < o22;
    if (s1 != s2) return s1 < s2;
    if (d11 != d21) return d11 < d21;
    return d12 < d22;
}
}

VisualizadorAutomataWidget::VisualizadorAutomataWidget(QWidget* parent)
    : QWidget(parent), escena(new QGraphicsScene(this)), vista(new QGraphicsView(escena, this)),
      dfaActual(nullptr), unionActual(nullptr), mostrandoUnion(false) {
    construirControles();
}

void VisualizadorAutomataWidget::construirControles() {
    QVBoxLayout* principal = new QVBoxLayout(this);
    principal->setContentsMargins(0, 0, 0, 0);
    principal->setSpacing(6);
    QHBoxLayout* controles = new QHBoxLayout;
    QPushButton* menos = new QPushButton("-");
    QPushButton* ajustar = new QPushButton("Ajustar");
    QPushButton* mas = new QPushButton("+");
    menos->setToolTip("Reducir zoom");
    ajustar->setToolTip("Ajustar diagrama");
    mas->setToolTip("Aumentar zoom");
    controles->addStretch();
    controles->addWidget(menos);
    controles->addWidget(ajustar);
    controles->addWidget(mas);
    controles->addStretch();
    principal->addLayout(controles);
    vista->setMinimumHeight(380);
    vista->setRenderHint(QPainter::Antialiasing);
    vista->setDragMode(QGraphicsView::ScrollHandDrag);
    vista->setBackgroundBrush(QColor("#F8FAFC"));
    principal->addWidget(vista, 1);
    connect(menos, &QPushButton::clicked, this, [this]() { vista->scale(0.85, 0.85); });
    connect(ajustar, &QPushButton::clicked, this, [this]() { ajustarVista(); });
    connect(mas, &QPushButton::clicked, this, [this]() { vista->scale(1.18, 1.18); });
}

void VisualizadorAutomataWidget::limpiar() {
    escena->clear();
    posiciones.limpiar();
    posicionesCompuestas.limpiar();
    dfaActual = nullptr;
    unionActual = nullptr;
    limpiarResaltado();
}

void VisualizadorAutomataWidget::limpiarResaltado() {
    estadoResaltado.clear(); origenResaltado.clear(); simboloResaltado.clear(); destinoResaltado.clear();
    origenUnion1.clear(); origenUnion2.clear(); simboloUnion.clear(); destinoUnion1.clear(); destinoUnion2.clear();
}

void VisualizadorAutomataWidget::mostrarDFA(const DFA& dfa) {
    limpiar();
    dfaActual = &dfa;
    mostrandoUnion = false;
    const int cantidad = dfa.obtenerEstados().cantidad();
    const double centroX = 360.0;
    const double centroY = 250.0;
    const double radio = cantidad <= 1 ? 0.0 : (cantidad == 2 ? 150.0 : 150.0 + cantidad * 18.0);
    const NodoEstado* estado = dfa.obtenerEstados().obtenerPrimero();
    int indice = 0;
    while (estado != nullptr) {
        double x = cantidad == 1 ? centroX - NODE_DIAMETER / 2.0 :
                   cantidad == 2 ? centroX - radio + indice * radio * 2.0 - NODE_DIAMETER / 2.0 :
                   centroX + radio * std::cos(2.0 * PI * indice / cantidad) - NODE_DIAMETER / 2.0;
        double y = cantidad <= 2 ? centroY - NODE_DIAMETER / 2.0 :
                   centroY + radio * std::sin(2.0 * PI * indice / cantidad) - NODE_DIAMETER / 2.0;
        posiciones.agregar(estado->nombre, x + NODE_DIAMETER / 2.0, y + NODE_DIAMETER / 2.0);
        estado = estado->siguiente;
        ++indice;
    }
    const NodoTransicion* transicion = dfa.obtenerTransiciones().obtenerPrimero();
    while (transicion != nullptr) {
        const NodoPosicionEstado* origen = posiciones.buscar(transicion->origen);
        const NodoPosicionEstado* destino = posiciones.buscar(transicion->destino);
        if (origen != nullptr && destino != nullptr) {
            int totalVariantes = 0;
            int variante = 0;
            const NodoTransicion* candidata = dfa.obtenerTransiciones().obtenerPrimero();
            while (candidata != nullptr) {
                if (mismaPareja(candidata->origen, candidata->destino,
                                transicion->origen, transicion->destino)) {
                    ++totalVariantes;
                    if (transicionMenorEnPareja(candidata->origen, candidata->simbolo, candidata->destino,
                                                transicion->origen, transicion->simbolo, transicion->destino)) {
                        ++variante;
                    }
                }
                candidata = candidata->siguiente;
            }
            const bool resaltada = transicion->origen == origenResaltado && transicion->simbolo == simboloResaltado && transicion->destino == destinoResaltado;
            dibujarArista(origen->x, origen->y, destino->x, destino->y, QString::fromStdString(transicion->simbolo), origen->estado == destino->estado, resaltada, NODE_DIAMETER, totalVariantes, variante);
        }
        transicion = transicion->siguiente;
    }
    estado = dfa.obtenerEstados().obtenerPrimero();
    while (estado != nullptr) {
        const NodoPosicionEstado* posicion = posiciones.buscar(estado->nombre);
        if (posicion != nullptr) dibujarEstado(posicion->x, posicion->y, QString::fromStdString(estado->nombre), dfa.tieneEstadoInicial() && dfa.obtenerEstadoInicial() == estado->nombre, dfa.obtenerEstadosFinales().existe(estado->nombre), NODE_DIAMETER);
        estado = estado->siguiente;
    }
    ajustarVista();
}

void VisualizadorAutomataWidget::mostrarDFAUnion(const DFAUnion& dfaUnion) {
    limpiar();
    unionActual = &dfaUnion;
    mostrandoUnion = true;
    const int cantidad = dfaUnion.obtenerEstados().cantidad();
    const double centroX = 430.0;
    const double centroY = 270.0;
    const double radio = cantidad <= 1 ? 0.0 : 190.0 + cantidad * 8.0;
    const NodoEstadoCompuesto* estado = dfaUnion.obtenerEstados().obtenerPrimero();
    int indice = 0;
    while (estado != nullptr) {
        const double angulo = cantidad == 1 ? 0.0 : 2.0 * PI * indice / cantidad;
        posicionesCompuestas.agregar(estado->estadoDFA1, estado->estadoDFA2,
            centroX + (cantidad == 1 ? 0.0 : radio * std::cos(angulo)),
            centroY + (cantidad == 1 ? 0.0 : radio * std::sin(angulo)));
        estado = estado->siguiente;
        ++indice;
    }
    const NodoTransicionCompuesta* transicion = dfaUnion.obtenerTransiciones().obtenerPrimero();
    while (transicion != nullptr) {
        const NodoPosicionEstadoCompuesto* origen = posicionesCompuestas.buscar(transicion->origenDFA1, transicion->origenDFA2);
        const NodoPosicionEstadoCompuesto* destino = posicionesCompuestas.buscar(transicion->destinoDFA1, transicion->destinoDFA2);
        if (origen != nullptr && destino != nullptr) {
            int totalVariantes = 0;
            int variante = 0;
            const NodoTransicionCompuesta* candidata = dfaUnion.obtenerTransiciones().obtenerPrimero();
            while (candidata != nullptr) {
                if (mismaParejaCompuesta(candidata->origenDFA1, candidata->origenDFA2,
                                         candidata->destinoDFA1, candidata->destinoDFA2,
                                         transicion->origenDFA1, transicion->origenDFA2,
                                         transicion->destinoDFA1, transicion->destinoDFA2)) {
                    ++totalVariantes;
                    if (transicionMenorEnParejaCompuesta(
                            candidata->origenDFA1, candidata->origenDFA2, candidata->simbolo,
                            candidata->destinoDFA1, candidata->destinoDFA2,
                            transicion->origenDFA1, transicion->origenDFA2, transicion->simbolo,
                            transicion->destinoDFA1, transicion->destinoDFA2)) {
                        ++variante;
                    }
                }
                candidata = candidata->siguiente;
            }
            const bool resaltada = transicion->origenDFA1 == origenUnion1 && transicion->origenDFA2 == origenUnion2 && transicion->simbolo == simboloUnion && transicion->destinoDFA1 == destinoUnion1 && transicion->destinoDFA2 == destinoUnion2;
            dibujarArista(origen->x, origen->y, destino->x, destino->y, QString::fromStdString(transicion->simbolo), origen == destino, resaltada, 86.0, totalVariantes, variante);
        }
        transicion = transicion->siguiente;
    }
    estado = dfaUnion.obtenerEstados().obtenerPrimero();
    while (estado != nullptr) {
        const NodoPosicionEstadoCompuesto* posicion = posicionesCompuestas.buscar(estado->estadoDFA1, estado->estadoDFA2);
        if (posicion != nullptr) {
            const bool inicial = dfaUnion.tieneEstadoInicial() && estado->estadoDFA1 == dfaUnion.obtenerEstadoInicialDFA1() && estado->estadoDFA2 == dfaUnion.obtenerEstadoInicialDFA2();
            dibujarEstado(posicion->x, posicion->y, estadoCompuesto(estado->estadoDFA1, estado->estadoDFA2), inicial, dfaUnion.obtenerEstadosFinales().existe(estado->estadoDFA1, estado->estadoDFA2), 86.0);
        }
        estado = estado->siguiente;
    }
    ajustarVista();
}

void VisualizadorAutomataWidget::dibujarEstado(double x, double y, const QString& nombre, bool inicial, bool final, double diametro) {
    const bool resaltado = mostrandoUnion ? nombre == estadoCompuesto(origenUnion1, origenUnion2) : nombre.toStdString() == estadoResaltado;
    QBrush relleno(resaltado ? QColor("#DBEAFE") : QColor("#FFFFFF"));
    QPen borde(QColor("#2563EB"), resaltado ? 3 : 2);
    QGraphicsEllipseItem* circulo = escena->addEllipse(x - diametro / 2.0, y - diametro / 2.0, diametro, diametro, borde, relleno);
    circulo->setZValue(3);
    if (final) {
        QGraphicsEllipseItem* circuloInterior = escena->addEllipse(x - diametro / 2.0 + 6, y - diametro / 2.0 + 6, diametro - 12, diametro - 12, QPen(QColor("#2563EB"), 2), Qt::NoBrush);
        circuloInterior->setZValue(3);
    }
    QGraphicsTextItem* texto = escena->addText(nombre);
    texto->setDefaultTextColor(QColor("#0F172A"));
    QRectF rect = texto->boundingRect();
    texto->setPos(x - rect.width() / 2.0, y - rect.height() / 2.0);
    texto->setZValue(4);
    if (inicial) dibujarFlechaEntrada(x, y, diametro);
}

void VisualizadorAutomataWidget::dibujarFlechaEntrada(double x, double y, double diametro) {
    QPen pen(QColor("#2563EB"), 2);
    QGraphicsLineItem* linea = escena->addLine(x - diametro / 2.0 - 48, y, x - diametro / 2.0 - 4, y, pen);
    linea->setZValue(0);
    QPolygonF punta;
    punta << QPointF(x - diametro / 2.0 - 4, y) << QPointF(x - diametro / 2.0 - 14, y - 6) << QPointF(x - diametro / 2.0 - 14, y + 6);
    QGraphicsPolygonItem* flecha = escena->addPolygon(punta, pen, QBrush(QColor("#2563EB")));
    flecha->setZValue(0);
}

void VisualizadorAutomataWidget::dibujarPuntaFlecha(const QPointF& punta, double anguloGrados,
                                                    const QPen& pen, double tamano) {
    const double angulo = anguloGrados * PI / 180.0;
    QPolygonF cabeza;
    cabeza << punta
           << QPointF(punta.x() - tamano * std::cos(angulo - 0.42),
                      punta.y() - tamano * std::sin(angulo - 0.42))
           << QPointF(punta.x() - tamano * std::cos(angulo + 0.42),
                      punta.y() - tamano * std::sin(angulo + 0.42));
    QGraphicsPolygonItem* item = escena->addPolygon(cabeza, QPen(pen.color(), 1.2), QBrush(pen.color()));
    item->setZValue(1);
}

void VisualizadorAutomataWidget::dibujarArista(double x1, double y1, double x2, double y2,
                                               const QString& simbolo, bool loop, bool resaltada,
                                               double diametro, int totalVariantes, int variante) {
    QPen pen(resaltada ? QColor("#16A34A") : QColor("#64748B"), resaltada ? 3 : 2);
    QPainterPath camino;
    QPointF punta;
    double etiquetaX = 0.0;
    double etiquetaY = 0.0;

    if (loop) {
        const double radio = diametro / 2.0;
        const int region = variante % 4;
        const double ciclo = static_cast<double>(variante / 4);
        const double escala = 1.0 + 0.4 * ciclo;
        double anguloInicio = 0.0;
        double anguloFin = 0.0;
        double dxControl1 = 0.0, dyControl1 = 0.0, dxControl2 = 0.0, dyControl2 = 0.0;
        switch (region) {
        case 0:
            anguloInicio = 205.0; anguloFin = 335.0;
            dxControl1 = -0.4; dyControl1 = -2.2;
            dxControl2 = 0.4; dyControl2 = -2.2;
            break;
        case 1:
            anguloInicio = 25.0; anguloFin = 155.0;
            dxControl1 = 0.4; dyControl1 = 2.2;
            dxControl2 = -0.4; dyControl2 = 2.2;
            break;
        case 2:
            anguloInicio = 100.0; anguloFin = 260.0;
            dxControl1 = -2.2; dyControl1 = 0.4;
            dxControl2 = -2.2; dyControl2 = -0.4;
            break;
        default:
            anguloInicio = 280.0; anguloFin = 80.0;
            dxControl1 = 2.2; dyControl1 = -0.4;
            dxControl2 = 2.2; dyControl2 = 0.4;
            break;
        }
        const double radInicio = anguloInicio * PI / 180.0;
        const double radFin = anguloFin * PI / 180.0;
        const QPointF inicio(x1 + radio * std::cos(radInicio), y1 + radio * std::sin(radInicio));
        const QPointF fin(x1 + radio * std::cos(radFin), y1 + radio * std::sin(radFin));
        const double ext = radio * 2.2 * escala;
        const QPointF control1(x1 + dxControl1 * ext, y1 + dyControl1 * ext);
        const QPointF control2(x1 + dxControl2 * ext, y1 + dyControl2 * ext);
        camino.moveTo(inicio);
        camino.cubicTo(control1, control2, fin);
        punta = fin;
        const double extEtiqueta = radio * 2.4 * escala;
        switch (region) {
        case 0:
            etiquetaX = x1; etiquetaY = y1 - extEtiqueta;
            break;
        case 1:
            etiquetaX = x1; etiquetaY = y1 + extEtiqueta;
            break;
        case 2:
            etiquetaX = x1 - extEtiqueta; etiquetaY = y1;
            break;
        default:
            etiquetaX = x1 + extEtiqueta; etiquetaY = y1;
            break;
        }
    } else {
        const double dx = x2 - x1;
        const double dy = y2 - y1;
        const double distancia = std::sqrt(dx * dx + dy * dy);
        if (distancia < 1.0e-9) return;
        const double angulo = std::atan2(dy, dx);
        const QPointF inicio = puntoBorde(x1, y1, diametro, angulo);
        const QPointF fin = puntoBorde(x2, y2, diametro, angulo + PI);
        const QPointF puntoMedio((inicio.x() + fin.x()) / 2.0, (inicio.y() + fin.y()) / 2.0);
        double px = -dy / distancia;
        double py = dx / distancia;
        if (py > 0.0 || (py == 0.0 && px < 0.0)) {
            px = -px;
            py = -py;
        }
        const double separacion = totalVariantes > 1 ? 42.0 : 0.0;
        const double desvio = (variante - (static_cast<double>(totalVariantes) - 1.0) / 2.0) * separacion;
        const QPointF control(puntoMedio.x() + px * desvio, puntoMedio.y() + py * desvio);
        camino.moveTo(inicio);
        if (desvio == 0.0) {
            camino.lineTo(fin);
        } else {
            camino.quadTo(control, fin);
        }
        punta = fin;
        double desplazamientoEtiqueta = 16.0;
        if (desvio > 0.0) desplazamientoEtiqueta = desvio + 20.0;
        else if (desvio < 0.0) desplazamientoEtiqueta = desvio - 20.0;
        etiquetaX = puntoMedio.x() + px * desplazamientoEtiqueta;
        etiquetaY = puntoMedio.y() + py * desplazamientoEtiqueta;
    }

    QGraphicsPathItem* arista = escena->addPath(camino, pen);
    arista->setZValue(0);
    dibujarPuntaFlecha(punta, camino.angleAtPercent(1.0), pen, 12.0);

    QGraphicsTextItem* texto = escena->addText(simbolo);
    texto->setDefaultTextColor(resaltada ? QColor("#15803D") : QColor("#334155"));
    const QRectF rectTexto = texto->boundingRect();
    const QPointF posicionEtiqueta(etiquetaX - rectTexto.width() / 2.0,
                                   etiquetaY - rectTexto.height() / 2.0);
    const QRectF fondo = QRectF(posicionEtiqueta.x(), posicionEtiqueta.y(),
                                rectTexto.width(), rectTexto.height()).adjusted(-4, -2, 4, 2);
    QGraphicsRectItem* fondoEtiqueta = escena->addRect(fondo, QPen(QColor("#E2E8F0")), QBrush(QColor("#FFFFFF")));
    fondoEtiqueta->setZValue(1);
    texto->setPos(posicionEtiqueta);
    texto->setZValue(2);
}

void VisualizadorAutomataWidget::ajustarVista() {
    if (!escena->items().isEmpty()) vista->fitInView(escena->itemsBoundingRect().adjusted(-35, -35, 35, 35), Qt::KeepAspectRatio);
}

void VisualizadorAutomataWidget::resaltarEstadoDFA(const std::string& estado) {
    estadoResaltado = estado;
    aplicarResaltadoDFA();
}

void VisualizadorAutomataWidget::resaltarTransicionDFA(const std::string& origen, const std::string& simbolo, const std::string& destino) {
    origenResaltado = origen; simboloResaltado = simbolo; destinoResaltado = destino; aplicarResaltadoDFA();
}

void VisualizadorAutomataWidget::resaltarEstadoUnion(const std::string& estadoDFA1, const std::string& estadoDFA2) {
    origenUnion1 = estadoDFA1; origenUnion2 = estadoDFA2; aplicarResaltadoUnion();
}

void VisualizadorAutomataWidget::resaltarTransicionUnion(const std::string& origenDFA1, const std::string& origenDFA2, const std::string& simbolo, const std::string& destinoDFA1, const std::string& destinoDFA2) {
    origenUnion1 = origenDFA1; origenUnion2 = origenDFA2; simboloUnion = simbolo; destinoUnion1 = destinoDFA1; destinoUnion2 = destinoDFA2; aplicarResaltadoUnion();
}

void VisualizadorAutomataWidget::aplicarResaltadoDFA() { if (dfaActual != nullptr) mostrarDFA(*dfaActual); }
void VisualizadorAutomataWidget::aplicarResaltadoUnion() { if (unionActual != nullptr) mostrarDFAUnion(*unionActual); }
void VisualizadorAutomataWidget::actualizarEscena() { if (mostrandoUnion && unionActual != nullptr) mostrarDFAUnion(*unionActual); else if (dfaActual != nullptr) mostrarDFA(*dfaActual); }

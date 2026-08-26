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
            bool inversa = false;
            const NodoTransicion* candidata = dfa.obtenerTransiciones().obtenerPrimero();
            while (candidata != nullptr) {
                if (candidata->origen == transicion->destino && candidata->destino == transicion->origen) inversa = true;
                candidata = candidata->siguiente;
            }
            const bool resaltada = transicion->origen == origenResaltado && transicion->simbolo == simboloResaltado && transicion->destino == destinoResaltado;
            dibujarArista(origen->x, origen->y, destino->x, destino->y, QString::fromStdString(transicion->simbolo), inversa && transicion->origen < transicion->destino, origen->estado == destino->estado, resaltada, NODE_DIAMETER);
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
            const bool resaltada = transicion->origenDFA1 == origenUnion1 && transicion->origenDFA2 == origenUnion2 && transicion->simbolo == simboloUnion && transicion->destinoDFA1 == destinoUnion1 && transicion->destinoDFA2 == destinoUnion2;
            dibujarArista(origen->x, origen->y, destino->x, destino->y, QString::fromStdString(transicion->simbolo), false, origen == destino, resaltada, 86.0);
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

void VisualizadorAutomataWidget::dibujarArista(double x1, double y1, double x2, double y2, const QString& simbolo, bool curvaArriba, bool loop, bool resaltada, double diametro) {
    QPen pen(resaltada ? QColor("#16A34A") : QColor("#64748B"), resaltada ? 3 : 2);
    QPainterPath camino;
    if (loop) {
        camino.moveTo(x1 - diametro * 0.25, y1 - diametro * 0.35);
        camino.cubicTo(x1 - diametro * 0.9, y1 - diametro * 1.2, x1 + diametro * 0.9, y1 - diametro * 1.2, x1 + diametro * 0.25, y1 - diametro * 0.35);
    } else {
        const double dx = x2 - x1;
        const double dy = y2 - y1;
        const double distancia = std::sqrt(dx * dx + dy * dy);
        const double angulo = std::atan2(dy, dx);
        const QPointF inicio = puntoBorde(x1, y1, diametro, angulo);
        const QPointF fin = puntoBorde(x2, y2, diametro, angulo + PI);
        camino.moveTo(inicio);
        if (curvaArriba) {
            const QPointF control((x1 + x2) / 2.0, (y1 + y2) / 2.0 - 38.0);
            camino.quadTo(control, fin);
        } else {
            camino.lineTo(fin);
        }
        Q_UNUSED(distancia);
    }
    QGraphicsPathItem* arista = escena->addPath(camino, pen);
    arista->setZValue(0);
    QPointF punta = loop ? QPointF(x1 + diametro * 0.25, y1 - diametro * 0.35) : camino.pointAtPercent(1.0);
    QGraphicsLineItem* marca = escena->addLine(punta.x() - 8, punta.y() - 5, punta.x(), punta.y(), pen);
    QGraphicsLineItem* marcaInferior = escena->addLine(punta.x() - 8, punta.y() + 5, punta.x(), punta.y(), pen);
    marca->setZValue(0);
    marcaInferior->setZValue(0);
    Q_UNUSED(marca);
    QGraphicsTextItem* texto = escena->addText(simbolo);
    texto->setDefaultTextColor(resaltada ? QColor("#15803D") : QColor("#334155"));
    const QRectF rectTexto = texto->boundingRect();
    double textoX = 0.0;
    double textoY = 0.0;
    if (loop) {
        textoX = x1 - rectTexto.width() / 2.0;
        textoY = y1 - diametro * 1.45 - rectTexto.height() / 2.0;
    } else {
        const double puntoMedioX = (x1 + x2) / 2.0;
        const double puntoMedioY = (y1 + y2) / 2.0;
        textoX = puntoMedioX - rectTexto.width() / 2.0;
        textoY = puntoMedioY - rectTexto.height() / 2.0 - (curvaArriba ? 38.0 : 12.0);
    }
    const QRectF fondo = QRectF(textoX, textoY, rectTexto.width(), rectTexto.height()).adjusted(-4, -2, 4, 2);
    QGraphicsRectItem* fondoEtiqueta = escena->addRect(fondo, QPen(QColor("#E2E8F0")), QBrush(QColor("#FFFFFF")));
    fondoEtiqueta->setZValue(1);
    texto->setPos(textoX, textoY);
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

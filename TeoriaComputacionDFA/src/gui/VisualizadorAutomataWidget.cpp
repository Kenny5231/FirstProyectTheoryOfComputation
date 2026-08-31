#include "gui/VisualizadorAutomataWidget.h"

#include "automata/DFA.h"
#include "automata/DFAUnion.h"
#include "estructuras/NodoEstado.h"
#include "estructuras/NodoEstadoCompuesto.h"
#include "estructuras/NodoSimbolo.h"
#include "estructuras/NodoTransicion.h"
#include "estructuras/NodoTransicionCompuesta.h"

#include <QBrush>
#include <QColor>
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
#include <QFont>
#include <QFontMetrics>
#include <cmath>

namespace {
const double PI = 3.14159265358979323846;
const double NODE_DIAMETER = 70.0;
const QColor COLOR_AZUL("#2563EB");
const QColor COLOR_AZUL_OSCURO("#1E3A8A");
const QColor COLOR_TEXTO("#0F172A");
const QColor COLOR_ARISTA("#64748B");
const QColor COLOR_FINAL("#14B8A6");
const QColor COLOR_RESALTADO("#16A34A");
const double MARGEN_IZQUIERDO = 150.0;
const double MARGEN_DERECHO = 140.0;
const double MARGEN_VERTICAL = 115.0;
const double ALTO_MINIMO_ESCENA = 420.0;

QString estadoCompuesto(const std::string& primero, const std::string& segundo) {
    return "(" + QString::fromStdString(primero) + "," + QString::fromStdString(segundo) + ")";
}

bool transicionDFAAnteriorMismoGrupo(const DFA& dfa, const NodoTransicion* transicion) {
    const NodoTransicion* actual = dfa.obtenerTransiciones().obtenerPrimero();
    while (actual != nullptr && actual != transicion) {
        if (actual->origen == transicion->origen && actual->destino == transicion->destino) {
            return true;
        }
        actual = actual->siguiente;
    }
    return false;
}

QString etiquetaGrupoDFA(const DFA& dfa, const std::string& origen, const std::string& destino) {
    QString etiqueta;
    const NodoTransicion* actual = dfa.obtenerTransiciones().obtenerPrimero();
    while (actual != nullptr) {
        if (actual->origen == origen && actual->destino == destino) {
            if (!etiqueta.isEmpty()) etiqueta += ", ";
            etiqueta += QString::fromStdString(actual->simbolo);
        }
        actual = actual->siguiente;
    }
    return etiqueta;
}

bool grupoDFAResaltado(const DFA& dfa, const std::string& origen, const std::string& destino,
                       const std::string& origenResaltado, const std::string& simboloResaltado,
                       const std::string& destinoResaltado) {
    const NodoTransicion* actual = dfa.obtenerTransiciones().obtenerPrimero();
    while (actual != nullptr) {
        if (actual->origen == origen && actual->destino == destino &&
            actual->origen == origenResaltado && actual->simbolo == simboloResaltado &&
            actual->destino == destinoResaltado) {
            return true;
        }
        actual = actual->siguiente;
    }
    return false;
}

bool existeDireccionInversaDFA(const DFA& dfa, const std::string& origen, const std::string& destino) {
    const NodoTransicion* actual = dfa.obtenerTransiciones().obtenerPrimero();
    while (actual != nullptr) {
        if (actual->origen == destino && actual->destino == origen) {
            return true;
        }
        actual = actual->siguiente;
    }
    return false;
}

bool transicionUnionAnteriorMismoGrupo(const DFAUnion& dfaUnion, const NodoTransicionCompuesta* transicion) {
    const NodoTransicionCompuesta* actual = dfaUnion.obtenerTransiciones().obtenerPrimero();
    while (actual != nullptr && actual != transicion) {
        if (actual->origenDFA1 == transicion->origenDFA1 &&
            actual->origenDFA2 == transicion->origenDFA2 &&
            actual->destinoDFA1 == transicion->destinoDFA1 &&
            actual->destinoDFA2 == transicion->destinoDFA2) {
            return true;
        }
        actual = actual->siguiente;
    }
    return false;
}

QString etiquetaGrupoUnion(const DFAUnion& dfaUnion, const std::string& origenDFA1,
                           const std::string& origenDFA2, const std::string& destinoDFA1,
                           const std::string& destinoDFA2) {
    QString etiqueta;
    const NodoTransicionCompuesta* actual = dfaUnion.obtenerTransiciones().obtenerPrimero();
    while (actual != nullptr) {
        if (actual->origenDFA1 == origenDFA1 && actual->origenDFA2 == origenDFA2 &&
            actual->destinoDFA1 == destinoDFA1 && actual->destinoDFA2 == destinoDFA2) {
            if (!etiqueta.isEmpty()) etiqueta += ", ";
            etiqueta += QString::fromStdString(actual->simbolo);
        }
        actual = actual->siguiente;
    }
    return etiqueta;
}

bool grupoUnionResaltado(const DFAUnion& dfaUnion, const std::string& origenDFA1,
                         const std::string& origenDFA2, const std::string& destinoDFA1,
                         const std::string& destinoDFA2, const std::string& origenUnion1,
                         const std::string& origenUnion2, const std::string& simboloUnion,
                         const std::string& destinoUnion1, const std::string& destinoUnion2) {
    const NodoTransicionCompuesta* actual = dfaUnion.obtenerTransiciones().obtenerPrimero();
    while (actual != nullptr) {
        if (actual->origenDFA1 == origenDFA1 && actual->origenDFA2 == origenDFA2 &&
            actual->destinoDFA1 == destinoDFA1 && actual->destinoDFA2 == destinoDFA2 &&
            actual->origenDFA1 == origenUnion1 && actual->origenDFA2 == origenUnion2 &&
            actual->simbolo == simboloUnion && actual->destinoDFA1 == destinoUnion1 &&
            actual->destinoDFA2 == destinoUnion2) {
            return true;
        }
        actual = actual->siguiente;
    }
    return false;
}

bool existeDireccionInversaUnion(const DFAUnion& dfaUnion, const std::string& origenDFA1,
                                 const std::string& origenDFA2, const std::string& destinoDFA1,
                                 const std::string& destinoDFA2) {
    const NodoTransicionCompuesta* actual = dfaUnion.obtenerTransiciones().obtenerPrimero();
    while (actual != nullptr) {
        if (actual->origenDFA1 == destinoDFA1 && actual->origenDFA2 == destinoDFA2 &&
            actual->destinoDFA1 == origenDFA1 && actual->destinoDFA2 == origenDFA2) {
            return true;
        }
        actual = actual->siguiente;
    }
    return false;
}

bool direccionUnionMenor(const std::string& origenDFA1, const std::string& origenDFA2,
                         const std::string& destinoDFA1, const std::string& destinoDFA2) {
    if (origenDFA1 != destinoDFA1) return origenDFA1 < destinoDFA1;
    if (origenDFA2 != destinoDFA2) return origenDFA2 < destinoDFA2;
    return false;
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
    const QString estiloBoton =
        "QPushButton { background-color: #FFFFFF; color: #1E293B; border: 1px solid #CBD5E1; "
        "border-radius: 8px; min-width: 96px; min-height: 30px; font-weight: 600; }"
        "QPushButton:hover { background-color: #EFF6FF; border-color: #93C5FD; color: #1D4ED8; }"
        "QPushButton:pressed { background-color: #DBEAFE; }";
    menos->setStyleSheet(estiloBoton);
    ajustar->setStyleSheet(estiloBoton);
    mas->setStyleSheet(estiloBoton);
    controles->addStretch();
    controles->addWidget(menos);
    controles->addWidget(ajustar);
    controles->addWidget(mas);
    controles->addStretch();
    principal->addLayout(controles);
    vista->setMinimumHeight(380);
    vista->setRenderHint(QPainter::Antialiasing);
    vista->setRenderHint(QPainter::TextAntialiasing);
    vista->setDragMode(QGraphicsView::ScrollHandDrag);
    vista->setBackgroundBrush(QColor("#EEF4FB"));
    vista->setStyleSheet("QGraphicsView { border: 1px solid #D8E2EE; border-radius: 10px; background: #EEF4FB; }");
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

double VisualizadorAutomataWidget::calcularDiametroEstado(const QString& nombre, double diametroBase) const {
    QFont fuente = vista->font();
    fuente.setBold(true);
    QFontMetrics metricas(fuente);
    const double diametroPorTexto = static_cast<double>(metricas.horizontalAdvance(nombre)) + 34.0;
    const double diametroPorAlto = static_cast<double>(metricas.height()) + 34.0;
    double diametro = diametroBase;
    if (diametroPorTexto > diametro) diametro = diametroPorTexto;
    if (diametroPorAlto > diametro) diametro = diametroPorAlto;
    return diametro;
}

int VisualizadorAutomataWidget::columnasSecundarias(int cantidadEstados) const {
    if (cantidadEstados <= 1) return 0;
    if (cantidadEstados <= 4) return 1;
    if (cantidadEstados <= 8) return 2;
    if (cantidadEstados <= 12) return 3;
    return 4;
}

int VisualizadorAutomataWidget::filasEnColumna(int cantidadRestante, int columnas, int columna) const {
    if (columnas <= 0 || cantidadRestante <= 0) return 0;
    const int base = cantidadRestante / columnas;
    const int sobrantes = cantidadRestante % columnas;
    return base + (columna <= sobrantes ? 1 : 0);
}

double VisualizadorAutomataWidget::separacionHorizontal(int cantidadEstados, double diametro) const {
    double separacion = 225.0;
    if (cantidadEstados <= 4) separacion = 310.0;
    else if (cantidadEstados <= 8) separacion = 270.0;
    else if (cantidadEstados <= 12) separacion = 245.0;
    if (diametro > 86.0) separacion += (diametro - 86.0) * 0.75;
    return separacion;
}

double VisualizadorAutomataWidget::separacionVertical(int maximoFilas, double diametro) const {
    double separacion = 130.0;
    if (maximoFilas <= 1) separacion = 0.0;
    else if (maximoFilas == 2) separacion = 200.0;
    else if (maximoFilas >= 4) separacion = 120.0;
    const double minima = diametro + 42.0;
    return separacion < minima ? minima : separacion;
}

void VisualizadorAutomataWidget::calcularPosicionesDFA(const DFA& dfa, double diametro) {
    const int cantidad = dfa.obtenerEstados().cantidad();
    if (cantidad <= 0) {
        escena->setSceneRect(0.0, 0.0, 700.0, ALTO_MINIMO_ESCENA);
        return;
    }

    const int columnas = columnasSecundarias(cantidad);
    const int cantidadRestante = cantidad - 1;
    int maximoFilas = 1;
    for (int columna = 1; columna <= columnas; ++columna) {
        const int filas = filasEnColumna(cantidadRestante, columnas, columna);
        if (filas > maximoFilas) maximoFilas = filas;
    }

    const double horizontal = separacionHorizontal(cantidad, diametro);
    const double vertical = separacionVertical(maximoFilas, diametro);
    double alto = MARGEN_VERTICAL * 2.0 + diametro;
    if (maximoFilas > 1) alto += (maximoFilas - 1) * vertical;
    if (alto < ALTO_MINIMO_ESCENA) alto = ALTO_MINIMO_ESCENA;
    const double centroY = alto / 2.0;
    const double xInicial = cantidad == 1 ? MARGEN_IZQUIERDO + 70.0 : MARGEN_IZQUIERDO;
    const double ancho = xInicial + horizontal * (columnas > 0 ? columnas : 1) + MARGEN_DERECHO + diametro;

    escena->setSceneRect(0.0, 0.0, ancho, alto);

    // El layout evita el círculo puro: el estado inicial real queda siempre
    // en la primera columna y los demás se reparten hacia la derecha.
    std::string estadoInicialVisual;
    if (dfa.tieneEstadoInicial()) {
        estadoInicialVisual = dfa.obtenerEstadoInicial();
    } else if (dfa.obtenerEstados().obtenerPrimero() != nullptr) {
        estadoInicialVisual = dfa.obtenerEstados().obtenerPrimero()->nombre;
    }

    posiciones.agregar(estadoInicialVisual, xInicial, centroY);

    const NodoEstado* estado = dfa.obtenerEstados().obtenerPrimero();
    int indiceRestante = 0;
    while (estado != nullptr) {
        if (estado->nombre != estadoInicialVisual) {
            int columnaAsignada = 1;
            int filaAsignada = 0;
            int acumulado = 0;
            for (int columna = 1; columna <= columnas; ++columna) {
                const int filas = filasEnColumna(cantidadRestante, columnas, columna);
                if (indiceRestante < acumulado + filas) {
                    columnaAsignada = columna;
                    filaAsignada = indiceRestante - acumulado;
                    break;
                }
                acumulado += filas;
            }
            const int filasColumna = filasEnColumna(cantidadRestante, columnas, columnaAsignada);
            const double x = xInicial + horizontal * columnaAsignada;
            const double y = centroY + (static_cast<double>(filaAsignada) - (static_cast<double>(filasColumna) - 1.0) / 2.0) * vertical;
            posiciones.agregar(estado->nombre, x, y);
            ++indiceRestante;
        }
        estado = estado->siguiente;
    }
}

void VisualizadorAutomataWidget::calcularPosicionesUnion(const DFAUnion& dfaUnion, double diametro) {
    const int cantidad = dfaUnion.obtenerEstados().cantidad();
    if (cantidad <= 0) {
        escena->setSceneRect(0.0, 0.0, 760.0, ALTO_MINIMO_ESCENA);
        return;
    }

    const int columnas = columnasSecundarias(cantidad);
    const int cantidadRestante = cantidad - 1;
    int maximoFilas = 1;
    for (int columna = 1; columna <= columnas; ++columna) {
        const int filas = filasEnColumna(cantidadRestante, columnas, columna);
        if (filas > maximoFilas) maximoFilas = filas;
    }

    const double horizontal = separacionHorizontal(cantidad, diametro);
    const double vertical = separacionVertical(maximoFilas, diametro);
    double alto = MARGEN_VERTICAL * 2.0 + diametro;
    if (maximoFilas > 1) alto += (maximoFilas - 1) * vertical;
    if (alto < ALTO_MINIMO_ESCENA) alto = ALTO_MINIMO_ESCENA;
    const double centroY = alto / 2.0;
    const double xInicial = cantidad == 1 ? MARGEN_IZQUIERDO + 80.0 : MARGEN_IZQUIERDO;
    const double ancho = xInicial + horizontal * (columnas > 0 ? columnas : 1) + MARGEN_DERECHO + diametro;

    escena->setSceneRect(0.0, 0.0, ancho, alto);

    // Para DFA Union se conserva el producto cartesiano ya calculado; solo
    // se cambia la ubicación visual, con q0U anclado a la izquierda.
    std::string inicialDFA1;
    std::string inicialDFA2;
    if (dfaUnion.tieneEstadoInicial()) {
        inicialDFA1 = dfaUnion.obtenerEstadoInicialDFA1();
        inicialDFA2 = dfaUnion.obtenerEstadoInicialDFA2();
    } else if (dfaUnion.obtenerEstados().obtenerPrimero() != nullptr) {
        inicialDFA1 = dfaUnion.obtenerEstados().obtenerPrimero()->estadoDFA1;
        inicialDFA2 = dfaUnion.obtenerEstados().obtenerPrimero()->estadoDFA2;
    }

    posicionesCompuestas.agregar(inicialDFA1, inicialDFA2, xInicial, centroY);

    const NodoEstadoCompuesto* estado = dfaUnion.obtenerEstados().obtenerPrimero();
    int indiceRestante = 0;
    while (estado != nullptr) {
        if (estado->estadoDFA1 != inicialDFA1 || estado->estadoDFA2 != inicialDFA2) {
            int columnaAsignada = 1;
            int filaAsignada = 0;
            int acumulado = 0;
            for (int columna = 1; columna <= columnas; ++columna) {
                const int filas = filasEnColumna(cantidadRestante, columnas, columna);
                if (indiceRestante < acumulado + filas) {
                    columnaAsignada = columna;
                    filaAsignada = indiceRestante - acumulado;
                    break;
                }
                acumulado += filas;
            }
            const int filasColumna = filasEnColumna(cantidadRestante, columnas, columnaAsignada);
            const double x = xInicial + horizontal * columnaAsignada;
            const double y = centroY + (static_cast<double>(filaAsignada) - (static_cast<double>(filasColumna) - 1.0) / 2.0) * vertical;
            posicionesCompuestas.agregar(estado->estadoDFA1, estado->estadoDFA2, x, y);
            ++indiceRestante;
        }
        estado = estado->siguiente;
    }
}

void VisualizadorAutomataWidget::mostrarDFA(const DFA& dfa) {
    limpiar();
    dfaActual = &dfa;
    mostrandoUnion = false;
    double diametro = NODE_DIAMETER;
    const NodoEstado* estado = dfa.obtenerEstados().obtenerPrimero();
    while (estado != nullptr) {
        const double necesario = calcularDiametroEstado(QString::fromStdString(estado->nombre), NODE_DIAMETER);
        if (necesario > diametro) diametro = necesario;
        estado = estado->siguiente;
    }
    calcularPosicionesDFA(dfa, diametro);
    const NodoTransicion* transicion = dfa.obtenerTransiciones().obtenerPrimero();
    while (transicion != nullptr) {
        if (!transicionDFAAnteriorMismoGrupo(dfa, transicion)) {
            const NodoPosicionEstado* origen = posiciones.buscar(transicion->origen);
            const NodoPosicionEstado* destino = posiciones.buscar(transicion->destino);
            if (origen != nullptr && destino != nullptr) {
                const bool loop = origen->estado == destino->estado;
                const bool inversa = !loop && existeDireccionInversaDFA(dfa, transicion->origen, transicion->destino);
                const int totalVariantes = inversa ? 2 : 1;
                const int variante = inversa && transicion->origen > transicion->destino ? 1 : 0;
                const bool resaltada = grupoDFAResaltado(dfa, transicion->origen, transicion->destino,
                                                         origenResaltado, simboloResaltado, destinoResaltado);
                dibujarArista(origen->x, origen->y, destino->x, destino->y,
                              etiquetaGrupoDFA(dfa, transicion->origen, transicion->destino),
                              loop, resaltada, diametro, totalVariantes, variante);
            }
        }
        transicion = transicion->siguiente;
    }
    estado = dfa.obtenerEstados().obtenerPrimero();
    while (estado != nullptr) {
        const NodoPosicionEstado* posicion = posiciones.buscar(estado->nombre);
        if (posicion != nullptr) dibujarEstado(posicion->x, posicion->y, QString::fromStdString(estado->nombre), dfa.tieneEstadoInicial() && dfa.obtenerEstadoInicial() == estado->nombre, dfa.obtenerEstadosFinales().existe(estado->nombre), diametro);
        estado = estado->siguiente;
    }
    ajustarVista();
}

void VisualizadorAutomataWidget::mostrarDFAUnion(const DFAUnion& dfaUnion) {
    limpiar();
    unionActual = &dfaUnion;
    mostrandoUnion = true;
    double diametro = 86.0;
    const NodoEstadoCompuesto* estado = dfaUnion.obtenerEstados().obtenerPrimero();
    while (estado != nullptr) {
        const double necesario = calcularDiametroEstado(estadoCompuesto(estado->estadoDFA1, estado->estadoDFA2), 86.0);
        if (necesario > diametro) diametro = necesario;
        estado = estado->siguiente;
    }
    calcularPosicionesUnion(dfaUnion, diametro);
    const NodoTransicionCompuesta* transicion = dfaUnion.obtenerTransiciones().obtenerPrimero();
    while (transicion != nullptr) {
        if (!transicionUnionAnteriorMismoGrupo(dfaUnion, transicion)) {
            const NodoPosicionEstadoCompuesto* origen = posicionesCompuestas.buscar(transicion->origenDFA1, transicion->origenDFA2);
            const NodoPosicionEstadoCompuesto* destino = posicionesCompuestas.buscar(transicion->destinoDFA1, transicion->destinoDFA2);
            if (origen != nullptr && destino != nullptr) {
                const bool loop = origen == destino;
                const bool inversa = !loop && existeDireccionInversaUnion(
                    dfaUnion, transicion->origenDFA1, transicion->origenDFA2,
                    transicion->destinoDFA1, transicion->destinoDFA2);
                const int totalVariantes = inversa ? 2 : 1;
                const int variante = inversa && !direccionUnionMenor(
                    transicion->origenDFA1, transicion->origenDFA2,
                    transicion->destinoDFA1, transicion->destinoDFA2) ? 1 : 0;
                const bool resaltada = grupoUnionResaltado(
                    dfaUnion, transicion->origenDFA1, transicion->origenDFA2,
                    transicion->destinoDFA1, transicion->destinoDFA2, origenUnion1,
                    origenUnion2, simboloUnion, destinoUnion1, destinoUnion2);
                dibujarArista(origen->x, origen->y, destino->x, destino->y,
                              etiquetaGrupoUnion(dfaUnion, transicion->origenDFA1, transicion->origenDFA2,
                                                 transicion->destinoDFA1, transicion->destinoDFA2),
                              loop, resaltada, diametro, totalVariantes, variante);
            }
        }
        transicion = transicion->siguiente;
    }
    estado = dfaUnion.obtenerEstados().obtenerPrimero();
    while (estado != nullptr) {
        const NodoPosicionEstadoCompuesto* posicion = posicionesCompuestas.buscar(estado->estadoDFA1, estado->estadoDFA2);
        if (posicion != nullptr) {
            const bool inicial = dfaUnion.tieneEstadoInicial() && estado->estadoDFA1 == dfaUnion.obtenerEstadoInicialDFA1() && estado->estadoDFA2 == dfaUnion.obtenerEstadoInicialDFA2();
            dibujarEstado(posicion->x, posicion->y, estadoCompuesto(estado->estadoDFA1, estado->estadoDFA2), inicial, dfaUnion.obtenerEstadosFinales().existe(estado->estadoDFA1, estado->estadoDFA2), diametro);
        }
        estado = estado->siguiente;
    }
    ajustarVista();
}

void VisualizadorAutomataWidget::dibujarEstado(double x, double y, const QString& nombre, bool inicial, bool final, double diametro) {
    const bool resaltado = mostrandoUnion ? nombre == estadoCompuesto(origenUnion1, origenUnion2) : nombre.toStdString() == estadoResaltado;
    QBrush relleno(resaltado ? QColor("#DCFCE7") : QColor("#FFFFFF"));
    const QColor colorBorde = resaltado ? COLOR_RESALTADO : (final ? COLOR_FINAL : COLOR_AZUL);
    QPen borde(colorBorde, resaltado ? 4 : 2.5);
    QGraphicsEllipseItem* sombra = escena->addEllipse(x - diametro / 2.0 + 4, y - diametro / 2.0 + 6,
                                                      diametro, diametro, Qt::NoPen,
                                                      QBrush(QColor(15, 23, 42, 26)));
    sombra->setZValue(2);
    QGraphicsEllipseItem* circulo = escena->addEllipse(x - diametro / 2.0, y - diametro / 2.0, diametro, diametro, borde, relleno);
    circulo->setZValue(3);
    if (final) {
        QGraphicsEllipseItem* circuloInterior = escena->addEllipse(x - diametro / 2.0 + 7, y - diametro / 2.0 + 7, diametro - 14, diametro - 14, QPen(colorBorde, 2.2), Qt::NoBrush);
        circuloInterior->setZValue(3);
    }
    if (inicial && mostrandoUnion) {
        QGraphicsEllipseItem* haloInicial = escena->addEllipse(
            x - diametro / 2.0 - 7, y - diametro / 2.0 - 7,
            diametro + 14, diametro + 14, QPen(COLOR_AZUL_OSCURO, 2.4),
            Qt::NoBrush);
        haloInicial->setZValue(2);

        QGraphicsTextItem* etiquetaInicial = escena->addText("q0U");
        QFont fuenteInicial = etiquetaInicial->font();
        fuenteInicial.setPointSize(8);
        fuenteInicial.setBold(true);
        etiquetaInicial->setFont(fuenteInicial);
        etiquetaInicial->setDefaultTextColor(QColor("#FFFFFF"));
        QRectF rectInicial = etiquetaInicial->boundingRect();
        const QRectF fondoInicial(
            x - rectInicial.width() / 2.0 - 8,
            y - diametro / 2.0 - 32,
            rectInicial.width() + 16,
            rectInicial.height() + 6);
        QGraphicsRectItem* badgeInicial = escena->addRect(
            fondoInicial, QPen(COLOR_AZUL_OSCURO, 1.2), QBrush(COLOR_AZUL_OSCURO));
        badgeInicial->setZValue(5);
        etiquetaInicial->setPos(
            fondoInicial.x() + (fondoInicial.width() - rectInicial.width()) / 2.0,
            fondoInicial.y() + (fondoInicial.height() - rectInicial.height()) / 2.0);
        etiquetaInicial->setZValue(6);
    }
    QGraphicsTextItem* texto = escena->addText(nombre);
    QFont fuente = texto->font();
    fuente.setPointSize(diametro > NODE_DIAMETER ? 10 : 9);
    fuente.setBold(true);
    texto->setFont(fuente);
    texto->setDefaultTextColor(COLOR_TEXTO);
    QRectF rect = texto->boundingRect();
    texto->setPos(x - rect.width() / 2.0, y - rect.height() / 2.0);
    texto->setZValue(4);
    if (inicial) dibujarFlechaEntrada(x, y, diametro);
}

void VisualizadorAutomataWidget::dibujarFlechaEntrada(double x, double y, double diametro) {
    QPen pen(COLOR_AZUL, 3);
    pen.setCapStyle(Qt::RoundCap);
    QGraphicsLineItem* linea = escena->addLine(x - diametro / 2.0 - 58, y, x - diametro / 2.0 - 5, y, pen);
    linea->setZValue(0);
    QPolygonF punta;
    punta << QPointF(x - diametro / 2.0 - 4, y) << QPointF(x - diametro / 2.0 - 17, y - 8) << QPointF(x - diametro / 2.0 - 17, y + 8);
    QGraphicsPolygonItem* flecha = escena->addPolygon(punta, pen, QBrush(COLOR_AZUL));
    flecha->setZValue(0);

    QGraphicsTextItem* etiqueta = escena->addText("inicio");
    QFont fuente = etiqueta->font();
    fuente.setPointSize(8);
    fuente.setBold(true);
    etiqueta->setFont(fuente);
    etiqueta->setDefaultTextColor(COLOR_AZUL_OSCURO);
    etiqueta->setPos(x - diametro / 2.0 - 58, y - 29);
    etiqueta->setZValue(2);
}

QPointF VisualizadorAutomataWidget::calcularPuntoBorde(const QPointF& centro,
                                                       const QPointF& direccionExterior,
                                                       double radio) const {
    const double longitud = std::sqrt(direccionExterior.x() * direccionExterior.x() +
                                      direccionExterior.y() * direccionExterior.y());
    if (longitud <= 1.0e-9) return centro;
    return QPointF(centro.x() + direccionExterior.x() * radio / longitud,
                   centro.y() + direccionExterior.y() * radio / longitud);
}

void VisualizadorAutomataWidget::dibujarPuntaFlecha(const QPointF& punta,
                                                    const QPointF& puntoAnterior,
                                                    const QPen& pen, double tamano) {
    double dx = punta.x() - puntoAnterior.x();
    double dy = punta.y() - puntoAnterior.y();
    const double longitud = std::sqrt(dx * dx + dy * dy);
    if (longitud <= 1.0e-9) return;
    dx /= longitud;
    dy /= longitud;
    const QPointF base(punta.x() - dx * tamano, punta.y() - dy * tamano);
    const QPointF perpendicular(-dy, dx);
    const double mitadAncho = tamano * 0.42;
    QPolygonF cabeza;
    cabeza << punta
           << QPointF(base.x() + perpendicular.x() * mitadAncho,
                      base.y() + perpendicular.y() * mitadAncho)
           << QPointF(base.x() - perpendicular.x() * mitadAncho,
                      base.y() - perpendicular.y() * mitadAncho);
    QGraphicsPolygonItem* item = escena->addPolygon(cabeza, QPen(pen.color(), 1.4), QBrush(pen.color()));
    item->setZValue(5);
}

void VisualizadorAutomataWidget::dibujarArista(double x1, double y1, double x2, double y2,
                                               const QString& simbolo, bool loop, bool resaltada,
                                               double diametro, int totalVariantes, int variante) {
    QPen pen(resaltada ? COLOR_RESALTADO : COLOR_ARISTA, resaltada ? 3.4 : 2.4);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    QPainterPath camino;
    QPointF punta;
    QPointF puntoAnteriorPunta;
    double etiquetaX = 0.0;
    double etiquetaY = 0.0;
    const double radio = diametro / 2.0;

    if (loop) {
        int region = variante % 4;
        if (variante == 0) {
            region = y1 > escena->sceneRect().center().y() + 20.0 ? 1 : 0;
        }
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
        puntoAnteriorPunta = camino.pointAtPercent(0.96);
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
        const QPointF centroOrigen(x1, y1);
        const QPointF centroDestino(x2, y2);
        const double dx = x2 - x1;
        const double dy = y2 - y1;
        const double distancia = std::sqrt(dx * dx + dy * dy);
        if (distancia < 1.0e-9) return;
        double px = -dy / distancia;
        double py = dx / distancia;
        if (py > 0.0 || (py == 0.0 && px < 0.0)) {
            px = -px;
            py = -py;
        }
        const bool mismaColumna = std::abs(dx) < diametro * 0.85;
        const double separacion = totalVariantes > 1 ? 56.0 : (mismaColumna ? 78.0 : 0.0);
        double desvio = (variante - (static_cast<double>(totalVariantes) - 1.0) / 2.0) * separacion;
        if (mismaColumna && totalVariantes == 1) desvio = separacion;
        const QPointF puntoMedioCentros((x1 + x2) / 2.0, (y1 + y2) / 2.0);
        const QPointF control(puntoMedioCentros.x() + px * desvio, puntoMedioCentros.y() + py * desvio);
        QPointF inicio = calcularPuntoBorde(centroOrigen, QPointF(dx, dy), radio);
        QPointF fin = calcularPuntoBorde(centroDestino, QPointF(-dx, -dy), radio);
        if (desvio != 0.0) {
            inicio = calcularPuntoBorde(centroOrigen, QPointF(control.x() - x1, control.y() - y1), radio);
            fin = calcularPuntoBorde(centroDestino, QPointF(control.x() - x2, control.y() - y2), radio);
        }
        const QPointF puntoMedio((inicio.x() + fin.x()) / 2.0, (inicio.y() + fin.y()) / 2.0);
        camino.moveTo(inicio);
        if (desvio == 0.0) {
            camino.lineTo(fin);
        } else {
            camino.quadTo(control, fin);
        }
        punta = fin;
        puntoAnteriorPunta = camino.pointAtPercent(desvio == 0.0 ? 0.94 : 0.97);
        double desplazamientoEtiqueta = 24.0;
        if (desvio > 0.0) desplazamientoEtiqueta = desvio + 24.0;
        else if (desvio < 0.0) desplazamientoEtiqueta = desvio - 24.0;
        etiquetaX = puntoMedio.x() + px * desplazamientoEtiqueta;
        etiquetaY = puntoMedio.y() + py * desplazamientoEtiqueta;
    }

    QGraphicsPathItem* arista = escena->addPath(camino, pen);
    arista->setZValue(0);
    dibujarPuntaFlecha(punta, puntoAnteriorPunta, pen, 16.0);

    QGraphicsTextItem* texto = escena->addText(simbolo);
    QFont fuente = texto->font();
    fuente.setPointSize(9);
    fuente.setBold(true);
    texto->setFont(fuente);
    texto->setDefaultTextColor(resaltada ? QColor("#14532D") : QColor("#1E293B"));
    const QRectF rectTexto = texto->boundingRect();
    const QPointF posicionEtiqueta(etiquetaX - rectTexto.width() / 2.0,
                                   etiquetaY - rectTexto.height() / 2.0);
    const QRectF fondo = QRectF(posicionEtiqueta.x(), posicionEtiqueta.y(),
                                rectTexto.width(), rectTexto.height()).adjusted(-8, -4, 8, 4);
    QGraphicsRectItem* fondoEtiqueta = escena->addRect(
        fondo,
        QPen(resaltada ? QColor("#86EFAC") : QColor("#D6E3F3"), 1.2),
        QBrush(resaltada ? QColor("#DCFCE7") : QColor("#FFFFFF")));
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

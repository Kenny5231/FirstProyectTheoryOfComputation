#ifndef VISUALIZADORAUTOMATAWIDGET_H
#define VISUALIZADORAUTOMATAWIDGET_H

#include <QWidget>
#include <string>

#include "estructuras/ListaPosicionesEstados.h"
#include "estructuras/ListaPosicionesEstadosCompuestos.h"

class DFA;
class DFAUnion;
class QPointF;
class QPen;
class QGraphicsScene;
class QGraphicsView;

class VisualizadorAutomataWidget : public QWidget {
public:
    explicit VisualizadorAutomataWidget(QWidget* parent = nullptr);

    void mostrarDFA(const DFA& dfa);
    void mostrarDFAUnion(const DFAUnion& dfaUnion);
    void limpiar();
    void ajustarVista();
    void resaltarEstadoDFA(const std::string& estado);
    void resaltarTransicionDFA(const std::string& origen, const std::string& simbolo,
                               const std::string& destino);
    void resaltarEstadoUnion(const std::string& estadoDFA1, const std::string& estadoDFA2);
    void resaltarTransicionUnion(const std::string& origenDFA1, const std::string& origenDFA2,
                                 const std::string& simbolo, const std::string& destinoDFA1,
                                 const std::string& destinoDFA2);

private:
    void construirControles();
    double calcularDiametroEstado(const QString& nombre, double diametroBase) const;
    int columnasSecundarias(int cantidadEstados) const;
    int filasEnColumna(int cantidadRestante, int columnas, int columna) const;
    double separacionHorizontal(int cantidadEstados, double diametro) const;
    double separacionVertical(int maximoFilas, double diametro) const;
    void calcularPosicionesDFA(const DFA& dfa, double diametro);
    void calcularPosicionesUnion(const DFAUnion& dfaUnion, double diametro);
    void dibujarDFA(const DFA& dfa);
    void dibujarUnion(const DFAUnion& dfaUnion);
    void dibujarEstado(double x, double y, const QString& nombre, bool inicial, bool final, double diametro);
    void dibujarArista(double x1, double y1, double x2, double y2, const QString& simbolo,
                       bool loop, bool resaltada, double diametro, int totalVariantes, int variante);
    QPointF calcularPuntoBorde(const QPointF& centro, const QPointF& direccionExterior, double radio) const;
    void dibujarPuntaFlecha(const QPointF& punta, const QPointF& puntoAnterior, const QPen& pen, double tamano);
    void dibujarFlechaEntrada(double x, double y, double diametro);
    void limpiarResaltado();
    void aplicarResaltadoDFA();
    void aplicarResaltadoUnion();
    void actualizarEscena();

    QGraphicsScene* escena;
    QGraphicsView* vista;
    const DFA* dfaActual;
    const DFAUnion* unionActual;
    bool mostrandoUnion;
    std::string estadoResaltado;
    std::string origenResaltado;
    std::string simboloResaltado;
    std::string destinoResaltado;
    std::string origenUnion1;
    std::string origenUnion2;
    std::string simboloUnion;
    std::string destinoUnion1;
    std::string destinoUnion2;
    ListaPosicionesEstados posiciones;
    ListaPosicionesEstadosCompuestos posicionesCompuestas;
};

#endif

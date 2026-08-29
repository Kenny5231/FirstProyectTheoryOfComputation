#ifndef VISTAPRUEBACADENAWIDGET_H
#define VISTAPRUEBACADENAWIDGET_H

#include <QWidget>
#include <string>

#include "estructuras/ListaPasosDFA.h"
#include "estructuras/ListaPasosDFAUnion.h"

class CadenaEntrada;
class DFA;
class DFAUnion;
class EditorDFAWidget;
class ListaPasosDFA;
class NodoPasoDFA;
class NodoPasoDFAUnion;
class QComboBox;
class QLabel;
class QLineEdit;
class QPlainTextEdit;
class QPushButton;
class VisualizadorAutomataWidget;
class VistaUnionDFAWidget;

class VistaPruebaCadenaWidget : public QWidget {
public:
    VistaPruebaCadenaWidget(DFA& dfa1, DFA& dfa2,
                            EditorDFAWidget* editorDFA1,
                            EditorDFAWidget* editorDFA2,
                            VistaUnionDFAWidget* vistaUnion,
                            QWidget* parent = nullptr);

    void actualizarDisponibilidad();
    void invalidarResultados();

private:
    enum AutomataSeleccionado {
        DFA1_SELECCIONADO = 0,
        DFA2_SELECCIONADO = 1,
        UNION_SELECCIONADO = 2
    };

    void crearInterfaz();
    void conectarEventos();
    void construirCadenaEntrada(const QString& texto, const DFA& dfa, CadenaEntrada& cadena) const;
    void construirCadenaEntradaUnion(const QString& texto, const DFAUnion& dfaUnion,
                                     CadenaEntrada& cadena) const;
    void evaluarCadena();
    void evaluarDFAIndividual();
    void evaluarUnion();
    void limpiarResultados();
    AutomataSeleccionado obtenerAutomataSeleccionado() const;
    DFA* obtenerDFASeleccionado() const;
    EditorDFAWidget* obtenerEditorSeleccionado() const;
    const NodoPasoDFA* obtenerPasoDFA(int indice) const;
    const NodoPasoDFAUnion* obtenerPasoUnion(int indice) const;
    bool todosSimbolosSonDeUnCaracter(const DFA& dfa) const;
    bool todosSimbolosEnAlfabeto(const DFAUnion& dfaUnion, const CadenaEntrada& cadena) const;
    bool estaEnEstadosFinalesUnion(const DFAUnion& dfaUnion,
                                   const std::string& estadoDFA1,
                                   const std::string& estadoDFA2) const;
    QString construirProcedimientoFormal(const DFA& dfa, const CadenaEntrada& cadena,
                                         const ListaPasosDFA& pasos, bool procesable,
                                         bool aceptada, const std::string& estadoFinal) const;
    QString construirProcedimientoFormalUnion(const DFAUnion& dfaUnion,
                                              const CadenaEntrada& cadena,
                                              const ListaPasosDFAUnion& pasos,
                                              bool procesable, bool aceptada,
                                              const std::string& estadoFinalDFA1,
                                              const std::string& estadoFinalDFA2) const;
    QString construirRecorridoSimple(const ListaPasosDFA& pasos) const;
    QString construirRecorridoSimpleUnion(const ListaPasosDFAUnion& pasos) const;
    QString simboloInvalidoUnion(const DFAUnion& dfaUnion, const CadenaEntrada& cadena) const;
    void actualizarEstiloResultado(QLabel* etiqueta, const QString& estado, bool error);
    void actualizarRecorrido();
    void cambiarDFASeleccionado();

    DFA* dfa1;
    DFA* dfa2;
    EditorDFAWidget* editorDFA1;
    EditorDFAWidget* editorDFA2;
    VistaUnionDFAWidget* vistaUnion;
    QComboBox* comboAutomata;
    QLineEdit* entradaCadena;
    QPushButton* botonEvaluar;
    QLabel* etiquetaEstadoDFA;
    QLabel* mensajeEstado;
    QLabel* resultadoLabel;
    QLabel* detallesResultadoLabel;
    QPlainTextEdit* procedimientoFormal;
    QPlainTextEdit* recorridoSimple;
    bool hayResultados;
    VisualizadorAutomataWidget* visualizador;
    ListaPasosDFA pasosDFAVisual;
    ListaPasosDFAUnion pasosUnionVisual;
    int pasoActual;
    QLabel* indicadorPaso;
    QPushButton* botonAnterior;
    QPushButton* botonSiguiente;
    QPushButton* botonReiniciar;
};

#endif

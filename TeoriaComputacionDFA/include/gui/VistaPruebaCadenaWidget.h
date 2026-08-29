#ifndef VISTAPRUEBACADENAWIDGET_H
#define VISTAPRUEBACADENAWIDGET_H

#include <QWidget>
#include <string>

#include "estructuras/ListaPasosDFA.h"

class CadenaEntrada;
class DFA;
class EditorDFAWidget;
class ListaPasosDFA;
class NodoPasoDFA;
class QComboBox;
class QLabel;
class QLineEdit;
class QPlainTextEdit;
class QPushButton;
class VisualizadorAutomataWidget;

class VistaPruebaCadenaWidget : public QWidget {
public:
    VistaPruebaCadenaWidget(DFA& dfa1, DFA& dfa2,
                            EditorDFAWidget* editorDFA1,
                            EditorDFAWidget* editorDFA2,
                            QWidget* parent = nullptr);

    void actualizarDisponibilidad();
    void invalidarResultados();

private:
    void crearInterfaz();
    void conectarEventos();
    void construirCadenaEntrada(const QString& texto, const DFA& dfa, CadenaEntrada& cadena) const;
    void evaluarCadena();
    void limpiarResultados();
    DFA* obtenerDFASeleccionado() const;
    EditorDFAWidget* obtenerEditorSeleccionado() const;
    const NodoPasoDFA* obtenerPasoDFA(int indice) const;
    bool todosSimbolosSonDeUnCaracter(const DFA& dfa) const;
    QString construirProcedimientoFormal(const DFA& dfa, const CadenaEntrada& cadena,
                                         const ListaPasosDFA& pasos, bool procesable,
                                         bool aceptada, const std::string& estadoFinal) const;
    QString construirRecorridoSimple(const ListaPasosDFA& pasos) const;
    void actualizarEstiloResultado(QLabel* etiqueta, const QString& estado, bool error);
    void actualizarRecorrido();
    void cambiarDFASeleccionado();

    DFA* dfa1;
    DFA* dfa2;
    EditorDFAWidget* editorDFA1;
    EditorDFAWidget* editorDFA2;
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
    int pasoActual;
    QLabel* indicadorPaso;
    QPushButton* botonAnterior;
    QPushButton* botonSiguiente;
    QPushButton* botonReiniciar;
};

#endif

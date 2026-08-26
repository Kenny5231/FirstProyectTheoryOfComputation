#ifndef VISTAPRUEBACADENAWIDGET_H
#define VISTAPRUEBACADENAWIDGET_H

#include <QWidget>
#include <string>

class CadenaEntrada;
class DFA;
class DFAUnion;
class EditorDFAWidget;
class ListaPasosDFA;
class ListaPasosDFAUnion;
class QLabel;
class QLineEdit;
class QPlainTextEdit;
class QPushButton;
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
    void crearInterfaz();
    void conectarEventos();
    void construirCadenaEntrada(const QString& texto, CadenaEntrada& cadena) const;
    void evaluarCadena();
    void limpiarResultados();
    QString construirTextoTrazaDFA(const DFA& dfa, const ListaPasosDFA& pasos,
                                   bool procesable, bool aceptada,
                                   const std::string& estadoFinal) const;
    QString construirTextoTrazaUnion(const DFAUnion& dfaUnion,
                                     const ListaPasosDFAUnion& pasos,
                                     bool procesable, bool aceptada,
                                     const std::string& estadoFinalDFA1,
                                     const std::string& estadoFinalDFA2) const;
    void actualizarEstiloResultado(QLabel* etiqueta, const QString& estado,
                                   bool error);

    DFA* dfa1;
    DFA* dfa2;
    EditorDFAWidget* editorDFA1;
    EditorDFAWidget* editorDFA2;
    VistaUnionDFAWidget* vistaUnion;
    QLineEdit* entradaCadena;
    QPushButton* botonEvaluar;
    QLabel* requisitoDFA1;
    QLabel* requisitoDFA2;
    QLabel* requisitoUnion;
    QLabel* mensajeEstado;
    QLabel* resultadoDFA1;
    QLabel* resultadoDFA2;
    QLabel* resultadoUnion;
    QLabel* estadoFinalDFA1Label;
    QLabel* estadoFinalDFA2Label;
    QLabel* estadoFinalUnionLabel;
    QLabel* consistenciaLabel;
    QPlainTextEdit* trazaDFA1;
    QPlainTextEdit* trazaDFA2;
    QPlainTextEdit* trazaUnion;
    bool hayResultados;
};

#endif

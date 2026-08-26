#ifndef VISTAUNIONDFAWIDGET_H
#define VISTAUNIONDFAWIDGET_H

#include <QWidget>
#include <QString>
#include <string>

class DFA;
class DFAUnion;
class EditorDFAWidget;
class QLabel;
class QPlainTextEdit;
class QPushButton;
class QTableWidget;

class VistaUnionDFAWidget : public QWidget {
public:
    VistaUnionDFAWidget(DFA& dfa1, DFA& dfa2,
                       EditorDFAWidget* editorDFA1,
                       EditorDFAWidget* editorDFA2,
                       QWidget* parent = nullptr);
    ~VistaUnionDFAWidget() override;

    bool hayUnionGenerada() const;
    const DFAUnion* obtenerDFAUnion() const;
    void actualizarDisponibilidad();
    void invalidarUnion();

private:
    void crearInterfaz();
    void conectarEventos();
    void generarUnion();
    void mostrarError(const QString& mensaje);
    void mostrarEstado(const QString& mensaje, bool correcto);
    void actualizarResultado();
    void limpiarResultado();
    QString textoEstados() const;
    QString textoSimbolos() const;
    QString textoFinales() const;
    QString textoEstadoInicial() const;
    void reconstruirTabla();
    bool buscarDestino(const std::string& origenDFA1,
                       const std::string& origenDFA2,
                       const std::string& simbolo,
                       std::string& destinoDFA1,
                       std::string& destinoDFA2) const;

    DFA* dfa1;
    DFA* dfa2;
    EditorDFAWidget* editorDFA1;
    EditorDFAWidget* editorDFA2;
    DFAUnion* dfaUnion;
    QLabel* estadoDFA1Label;
    QLabel* estadoDFA2Label;
    QLabel* estadoAlfabetosLabel;
    QLabel* mensajeLabel;
    QLabel* resumenEstadosLabel;
    QLabel* resumenSimbolosLabel;
    QLabel* resumenInicialLabel;
    QLabel* resumenFinalesLabel;
    QLabel* resumenTransicionesLabel;
    QLabel* estadosLabel;
    QLabel* simbolosLabel;
    QLabel* inicialLabel;
    QLabel* finalesLabel;
    QLabel* estadoResultadoLabel;
    QPlainTextEdit* panelErrores;
    QTableWidget* tablaTransiciones;
    QPushButton* botonGenerar;
};

#endif

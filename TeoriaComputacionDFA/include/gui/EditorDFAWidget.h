#ifndef EDITORDF_WIDGET_H
#define EDITORDF_WIDGET_H

#include <QWidget>

class DFA;
class QFrame;
class QComboBox;
class QLabel;
class QLineEdit;
class QPlainTextEdit;
class QPushButton;
class QTableWidget;
class VisualizadorAutomataWidget;

class EditorDFAWidget : public QWidget {
    Q_OBJECT

public:
    EditorDFAWidget(DFA& dfa, const QString& titulo, const QString& descripcion,
                    QWidget* parent = nullptr);

    bool estaValidado() const;
    bool esDFAValido() const;

signals:
    void estadoValidacionCambiado(bool valido);
    void dfaModificado();

private:
    void crearInterfaz(const QString& titulo, const QString& descripcion);
    QFrame* crearSeccion(const QString& titulo, const QString& simbolo);
    void conectarEventos();
    void refrescarInterfaz();
    void mostrarMensaje(const QString& texto, bool error);
    QString resumenEstados() const;
    QString resumenSimbolos() const;
    QString resumenFinales() const;
    void refrescarCombos();
    void refrescarTabla();
    void agregarEstado();
    void agregarSimbolo();
    void establecerInicial();
    void agregarFinal();
    void agregarTransicion();
    void eliminarTransicion(const QString& origen, const QString& simbolo,
                            const QString& destino);
    void validarDFA();
    void invalidarValidacion();

    DFA* dfa;
    QLineEdit* entradaEstado;
    QLineEdit* entradaSimbolo;
    QPushButton* botonAgregarEstado;
    QPushButton* botonAgregarSimbolo;
    QComboBox* comboEstadoInicial;
    QPushButton* botonEstablecerInicial;
    QComboBox* comboEstadoFinal;
    QPushButton* botonAgregarFinal;
    QComboBox* comboOrigen;
    QComboBox* comboSimboloTransicion;
    QComboBox* comboDestino;
    QPushButton* botonAgregarTransicion;
    QLabel* resumenEstadosLabel;
    QLabel* resumenSimbolosLabel;
    QLabel* resumenInicialLabel;
    QLabel* resumenFinalesLabel;
    QLabel* resumenTransicionesLabel;
    QLabel* inicialActualLabel;
    QLabel* listaEstadosLabel;
    QLabel* listaSimbolosLabel;
    QLabel* listaFinalesLabel;
    QLabel* mensajeEstado;
    QTableWidget* tablaTransiciones;
    VisualizadorAutomataWidget* visualizador;
    QFrame* tarjetaValidacion;
    QLabel* estadoValidacionLabel;
    QLabel* detalleValidacionLabel;
    QLabel* cantidadErroresLabel;
    QPlainTextEdit* panelErrores;
    QPushButton* botonValidar;
    bool validacionRealizada;
    bool dfaValido;
};

#endif

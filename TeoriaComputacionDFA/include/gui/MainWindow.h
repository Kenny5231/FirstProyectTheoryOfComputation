#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

#include "automata/DFA.h"

class EditorDFAWidget;
class VistaUnionDFAWidget;
class QStackedWidget;
class QPushButton;

class MainWindow : public QMainWindow {
public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    void configurarVentana();
    void crearInterfaz();
    QWidget* crearSidebar();
    QWidget* crearPaginaInicio();
    QWidget* crearPaginaDFA(const QString& titulo, const QString& subtitulo);
    QWidget* crearPaginaUnion();
    QWidget* crearPaginaPruebas();
    void conectarEventos();
    void mostrarPagina(int indice, QPushButton* botonActivo);
    void actualizarBotonActivo(QPushButton* botonActivo);

    QStackedWidget* paginas;
    QPushButton* botonInicio;
    QPushButton* botonDFA1;
    QPushButton* botonDFA2;
    QPushButton* botonUnion;
    QPushButton* botonPruebas;
    DFA dfa1;
    DFA dfa2;
    EditorDFAWidget* editorDFA1;
    EditorDFAWidget* editorDFA2;
    VistaUnionDFAWidget* vistaUnionDFA;
};

#endif

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

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
};

#endif

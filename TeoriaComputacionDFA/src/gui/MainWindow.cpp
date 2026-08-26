#include "gui/MainWindow.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSizePolicy>
#include <QStackedWidget>
#include <QStyle>
#include <QVBoxLayout>
#include <QWidget>

namespace {
QLabel* crearEtiqueta(const QString& texto, const QString& clase) {
    QLabel* etiqueta = new QLabel(texto);
    etiqueta->setProperty("class", clase);
    etiqueta->setWordWrap(true);
    return etiqueta;
}

QFrame* crearTarjeta(const QString& titulo, const QString& contenido) {
    QFrame* tarjeta = new QFrame;
    tarjeta->setProperty("class", "card");
    QVBoxLayout* layout = new QVBoxLayout(tarjeta);
    layout->setContentsMargins(22, 20, 22, 20);
    layout->setSpacing(8);
    layout->addWidget(crearEtiqueta(titulo, "cardTitle"));
    layout->addWidget(crearEtiqueta(contenido, "cardText"));
    return tarjeta;
}
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      paginas(nullptr),
      botonInicio(nullptr),
      botonDFA1(nullptr),
      botonDFA2(nullptr),
      botonUnion(nullptr),
      botonPruebas(nullptr) {
    configurarVentana();
    crearInterfaz();
    conectarEventos();
    actualizarBotonActivo(botonInicio);
}

void MainWindow::configurarVentana() {
    setWindowTitle("Sistema de Autómatas DFA");
    setMinimumSize(1000, 650);
    resize(1200, 750);
    setStyleSheet(
        "QMainWindow, QWidget#central { background: #F5F7FA; color: #111827; }"
        "QWidget#sidebar { background: #111827; }"
        "QLabel#brand { color: #FFFFFF; font-size: 20px; font-weight: 700; }"
        "QLabel#brandSub { color: #9CA3AF; font-size: 12px; }"
        "QLabel#sectionLabel { color: #9CA3AF; font-size: 11px; font-weight: 700; }"
        "QPushButton#navButton { background: transparent; border: 0; border-radius: 7px; "
        "color: #D1D5DB; text-align: left; padding: 13px 16px; font-size: 14px; }"
        "QPushButton#navButton:hover { background: #1F2937; color: #FFFFFF; }"
        "QPushButton#navButton[active=\"true\"] { background: #2563EB; color: #FFFFFF; font-weight: 700; }"
        "QLabel#headerTitle { color: #111827; font-size: 25px; font-weight: 700; }"
        "QLabel#headerSub, QLabel#pageSub { color: #6B7280; font-size: 14px; }"
        "QLabel#pageTitle { color: #111827; font-size: 28px; font-weight: 700; }"
        "QLabel#intro { color: #374151; font-size: 16px; }"
        "QFrame[class=\"card\"] { background: #FFFFFF; border: 1px solid #E5E7EB; border-radius: 8px; }"
        "QLabel#cardTitle { color: #111827; font-size: 16px; font-weight: 700; }"
        "QLabel#cardText { color: #6B7280; font-size: 13px; }"
        "QFrame#header { background: #FFFFFF; border-bottom: 1px solid #E5E7EB; }"
        "QLabel#eyebrow { color: #2563EB; font-size: 11px; font-weight: 700; }"
    );
}

void MainWindow::crearInterfaz() {
    QWidget* central = new QWidget(this);
    central->setObjectName("central");
    setCentralWidget(central);

    QVBoxLayout* principal = new QVBoxLayout(central);
    principal->setContentsMargins(0, 0, 0, 0);
    principal->setSpacing(0);

    QFrame* encabezado = new QFrame(central);
    encabezado->setObjectName("header");
    QHBoxLayout* encabezadoLayout = new QHBoxLayout(encabezado);
    encabezadoLayout->setContentsMargins(30, 20, 30, 20);
    encabezadoLayout->addWidget(crearEtiqueta("Sistema de Autómatas DFA", "headerTitle"));
    encabezadoLayout->addStretch();
    encabezadoLayout->addWidget(crearEtiqueta("Teoría de la Computación", "headerSub"));
    principal->addWidget(encabezado);

    QHBoxLayout* contenido = new QHBoxLayout;
    contenido->setContentsMargins(0, 0, 0, 0);
    contenido->setSpacing(0);
    contenido->addWidget(crearSidebar());

    paginas = new QStackedWidget(central);
    paginas->addWidget(crearPaginaInicio());
    paginas->addWidget(crearPaginaDFA("DFA 1", "Define los componentes del primer autómata."));
    paginas->addWidget(crearPaginaDFA("DFA 2", "Define los componentes del segundo autómata."));
    paginas->addWidget(crearPaginaUnion());
    paginas->addWidget(crearPaginaPruebas());
    contenido->addWidget(paginas, 1);
    principal->addLayout(contenido, 1);
}

QWidget* MainWindow::crearSidebar() {
    QWidget* sidebar = new QWidget;
    sidebar->setObjectName("sidebar");
    sidebar->setFixedWidth(230);
    QVBoxLayout* layout = new QVBoxLayout(sidebar);
    layout->setContentsMargins(18, 26, 18, 20);
    layout->setSpacing(8);
    layout->addWidget(crearEtiqueta("SISTEMA DFA", "brand"));
    layout->addWidget(crearEtiqueta("Laboratorio académico", "brandSub"));
    layout->addSpacing(28);
    layout->addWidget(crearEtiqueta("NAVEGACIÓN", "sectionLabel"));

    botonInicio = new QPushButton("Inicio", sidebar);
    botonDFA1 = new QPushButton("DFA 1", sidebar);
    botonDFA2 = new QPushButton("DFA 2", sidebar);
    botonUnion = new QPushButton("DFA Unión", sidebar);
    botonPruebas = new QPushButton("Probar cadena", sidebar);
    botonInicio->setObjectName("navButton");
    botonDFA1->setObjectName("navButton");
    botonDFA2->setObjectName("navButton");
    botonUnion->setObjectName("navButton");
    botonPruebas->setObjectName("navButton");
    botonInicio->setCursor(Qt::PointingHandCursor);
    botonDFA1->setCursor(Qt::PointingHandCursor);
    botonDFA2->setCursor(Qt::PointingHandCursor);
    botonUnion->setCursor(Qt::PointingHandCursor);
    botonPruebas->setCursor(Qt::PointingHandCursor);
    layout->addWidget(botonInicio);
    layout->addWidget(botonDFA1);
    layout->addWidget(botonDFA2);
    layout->addWidget(botonUnion);
    layout->addWidget(botonPruebas);
    layout->addStretch();
    layout->addWidget(crearEtiqueta("Fase 16\nInterfaz base", "brandSub"));
    return sidebar;
}

QWidget* MainWindow::crearPaginaInicio() {
    QWidget* pagina = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(pagina);
    layout->setContentsMargins(42, 38, 42, 38);
    layout->setSpacing(12);
    layout->addWidget(crearEtiqueta("BIENVENIDA", "eyebrow"));
    layout->addWidget(crearEtiqueta("Sistema de Autómatas Finitos Deterministas", "pageTitle"));
    layout->addWidget(crearEtiqueta("Construye, valida y analiza la unión de dos DFA.", "intro"));
    layout->addSpacing(24);
    QHBoxLayout* tarjetas = new QHBoxLayout;
    tarjetas->setSpacing(16);
    tarjetas->addWidget(crearTarjeta("01  Definir", "Configura los estados, el alfabeto y las transiciones de DFA 1 y DFA 2."));
    tarjetas->addWidget(crearTarjeta("02  Validar", "Comprueba que ambos autómatas cumplen las propiedades formales de un DFA."));
    tarjetas->addWidget(crearTarjeta("03  Analizar", "Explora la unión y prueba cadenas mediante sus recorridos."));
    layout->addLayout(tarjetas);
    layout->addStretch();
    return pagina;
}

QWidget* MainWindow::crearPaginaDFA(const QString& titulo, const QString& subtitulo) {
    QWidget* pagina = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(pagina);
    layout->setContentsMargins(42, 38, 42, 38);
    layout->setSpacing(12);
    layout->addWidget(crearEtiqueta("CONFIGURACIÓN", "eyebrow"));
    layout->addWidget(crearEtiqueta(titulo, "pageTitle"));
    layout->addWidget(crearEtiqueta(subtitulo, "pageSub"));
    layout->addSpacing(24);
    QHBoxLayout* fila = new QHBoxLayout;
    fila->setSpacing(16);
    fila->addWidget(crearTarjeta("Estados", "Configuración disponible en Fase 17"));
    fila->addWidget(crearTarjeta("Alfabeto", "Configuración disponible en Fase 17"));
    fila->addWidget(crearTarjeta("Estado inicial", "Configuración disponible en Fase 17"));
    layout->addLayout(fila);
    QHBoxLayout* filaDos = new QHBoxLayout;
    filaDos->setSpacing(16);
    filaDos->addWidget(crearTarjeta("Estados finales", "Configuración disponible en Fase 17"));
    filaDos->addWidget(crearTarjeta("Transiciones", "Configuración disponible en Fase 17"));
    filaDos->addStretch();
    layout->addLayout(filaDos);
    layout->addStretch();
    return pagina;
}

QWidget* MainWindow::crearPaginaUnion() {
    QWidget* pagina = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(pagina);
    layout->setContentsMargins(42, 38, 42, 38);
    layout->setSpacing(12);
    layout->addWidget(crearEtiqueta("OPERACIÓN", "eyebrow"));
    layout->addWidget(crearEtiqueta("DFA Unión", "pageTitle"));
    layout->addWidget(crearEtiqueta("Resultado de la unión de los dos autómatas validados.", "pageSub"));
    layout->addSpacing(24);
    QHBoxLayout* fila = new QHBoxLayout;
    fila->setSpacing(16);
    fila->addWidget(crearTarjeta("Estados compuestos", "Preparado para mostrar Q1 × Q2."));
    fila->addWidget(crearTarjeta("Estado inicial", "Se mostrará el par de estados iniciales."));
    fila->addWidget(crearTarjeta("Estados finales", "Se mostrarán los pares aceptores."));
    layout->addLayout(fila);
    layout->addWidget(crearTarjeta("Tabla de transiciones", "La unión estará disponible después de validar ambos DFA."));
    layout->addStretch();
    return pagina;
}

QWidget* MainWindow::crearPaginaPruebas() {
    QWidget* pagina = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(pagina);
    layout->setContentsMargins(42, 38, 42, 38);
    layout->setSpacing(12);
    layout->addWidget(crearEtiqueta("SIMULACIÓN", "eyebrow"));
    layout->addWidget(crearEtiqueta("Prueba de cadenas", "pageTitle"));
    layout->addWidget(crearEtiqueta("Evalúa una cadena en DFA 1, DFA 2 y DFA Unión.", "pageSub"));
    layout->addSpacing(24);
    QHBoxLayout* fila = new QHBoxLayout;
    fila->setSpacing(16);
    fila->addWidget(crearTarjeta("Entrada de cadena", "Disponible en Fase 17"));
    fila->addWidget(crearTarjeta("Trazabilidad", "Disponible en Fase 17"));
    fila->addStretch();
    layout->addLayout(fila);
    QHBoxLayout* resultados = new QHBoxLayout;
    resultados->setSpacing(16);
    resultados->addWidget(crearTarjeta("Resultado DFA 1", "Pendiente de conexión con el motor."));
    resultados->addWidget(crearTarjeta("Resultado DFA 2", "Pendiente de conexión con el motor."));
    resultados->addWidget(crearTarjeta("Resultado DFA Unión", "Pendiente de conexión con el motor."));
    layout->addLayout(resultados);
    layout->addStretch();
    return pagina;
}

void MainWindow::conectarEventos() {
    connect(botonInicio, &QPushButton::clicked, this, [this]() { mostrarPagina(0, botonInicio); });
    connect(botonDFA1, &QPushButton::clicked, this, [this]() { mostrarPagina(1, botonDFA1); });
    connect(botonDFA2, &QPushButton::clicked, this, [this]() { mostrarPagina(2, botonDFA2); });
    connect(botonUnion, &QPushButton::clicked, this, [this]() { mostrarPagina(3, botonUnion); });
    connect(botonPruebas, &QPushButton::clicked, this, [this]() { mostrarPagina(4, botonPruebas); });
}

void MainWindow::mostrarPagina(int indice, QPushButton* botonActivo) {
    paginas->setCurrentIndex(indice);
    actualizarBotonActivo(botonActivo);
}

void MainWindow::actualizarBotonActivo(QPushButton* botonActivo) {
    botonInicio->setProperty("active", botonInicio == botonActivo);
    botonDFA1->setProperty("active", botonDFA1 == botonActivo);
    botonDFA2->setProperty("active", botonDFA2 == botonActivo);
    botonUnion->setProperty("active", botonUnion == botonActivo);
    botonPruebas->setProperty("active", botonPruebas == botonActivo);
    botonInicio->style()->unpolish(botonInicio);
    botonInicio->style()->polish(botonInicio);
    botonDFA1->style()->unpolish(botonDFA1);
    botonDFA1->style()->polish(botonDFA1);
    botonDFA2->style()->unpolish(botonDFA2);
    botonDFA2->style()->polish(botonDFA2);
    botonUnion->style()->unpolish(botonUnion);
    botonUnion->style()->polish(botonUnion);
    botonPruebas->style()->unpolish(botonPruebas);
    botonPruebas->style()->polish(botonPruebas);
}

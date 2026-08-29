#include "gui/MainWindow.h"

#include "gui/EditorDFAWidget.h"
#include "gui/VistaUnionDFAWidget.h"
#include "gui/VistaPruebaCadenaWidget.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QStackedWidget>
#include <QStyle>
#include <QVBoxLayout>
#include <QWidget>

namespace {
QLabel* crearEtiqueta(const QString& texto, const QString& clase) {
    QLabel* etiqueta = new QLabel(texto);
    etiqueta->setObjectName(clase);
    etiqueta->setWordWrap(true);
    return etiqueta;
}

QFrame* crearTarjeta(const QString& titulo, const QString& contenido) {
    QFrame* tarjeta = new QFrame;
    tarjeta->setObjectName("card");
    QVBoxLayout* layout = new QVBoxLayout(tarjeta);
    layout->setContentsMargins(20, 18, 20, 18);
    layout->setSpacing(6);
    layout->addWidget(crearEtiqueta(titulo, "cardTitle"));
    layout->addWidget(crearEtiqueta(contenido, "cardText"));
    return tarjeta;
}

QFrame* crearTarjetaFlujo(const QString& numero, const QString& titulo,
                          const QString& contenido) {
    QFrame* tarjeta = new QFrame;
    tarjeta->setObjectName("card");
    QVBoxLayout* layout = new QVBoxLayout(tarjeta);
    layout->setContentsMargins(20, 18, 20, 18);
    layout->setSpacing(8);
    layout->addWidget(crearEtiqueta(numero, "stepNumber"));
    layout->addWidget(crearEtiqueta(titulo, "cardTitle"));
    layout->addWidget(crearEtiqueta(contenido, "cardText"));
    layout->addWidget(crearEtiqueta("●  Preparado", "badge"));
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
            botonPruebas(nullptr),
            dfa1(),
            dfa2(),
            editorDFA1(nullptr),
            editorDFA2(nullptr),
            vistaUnionDFA(nullptr),
            vistaPruebaCadena(nullptr) {
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
        "QMainWindow, QWidget#central { background: #F8FAFC; color: #0F172A; }"
        "QWidget#sidebar { background: #0F172A; }"
        "QLabel#brand { color: #FFFFFF; font-size: 19px; font-weight: 700; }"
        "QLabel#brandSub { color: #94A3B8; font-size: 12px; }"
        "QLabel#sectionLabel { color: #64748B; font-size: 11px; font-weight: 700; }"
        "QLabel#brandMark { background: #2563EB; color: #FFFFFF; border-radius: 8px; "
        "font-size: 16px; font-weight: 700; padding: 8px; }"
        "QPushButton#navButton { background: transparent; border: 0; border-radius: 7px; "
        "color: #CBD5E1; text-align: left; padding: 0 14px; min-height: 46px; "
        "font-size: 14px; }"
        "QPushButton#navButton:hover { background: #1E293B; color: #FFFFFF; }"
        "QPushButton#navButton[active=\"true\"] { background: #2563EB; color: #FFFFFF; font-weight: 700; }"
        "QLabel#headerTitle { color: #0F172A; font-size: 20px; font-weight: 700; }"
        "QLabel#headerSub, QLabel#pageSub { color: #64748B; font-size: 13px; }"
        "QLabel#headerMeta { color: #64748B; background: #F1F5F9; border-radius: 6px; "
        "padding: 7px 10px; font-size: 12px; }"
        "QLabel#pageTitle { color: #0F172A; font-size: 27px; font-weight: 700; }"
        "QLabel#intro { color: #334155; font-size: 15px; }"
        "QFrame#card { background: #FFFFFF; border: 1px solid #E2E8F0; border-radius: 12px; }"
        "QLabel#cardTitle { color: #0F172A; font-size: 15px; font-weight: 700; }"
        "QLabel#cardText { color: #64748B; font-size: 13px; }"
        "QLabel#stepNumber { color: #2563EB; font-size: 26px; font-weight: 700; }"
        "QLabel#badge { color: #15803D; background: #F0FDF4; border-radius: 5px; "
        "padding: 4px 7px; font-size: 11px; font-weight: 700; }"
        "QFrame#hero { background: #EFF6FF; border: 1px solid #DBEAFE; border-radius: 12px; }"
        "QLabel#heroTitle { color: #1E3A8A; font-size: 16px; font-weight: 700; }"
        "QLabel#heroText { color: #1E40AF; font-size: 13px; }"
        "QFrame#header { background: #FFFFFF; border-bottom: 1px solid #E2E8F0; }"
        "QLabel#eyebrow { color: #2563EB; font-size: 11px; font-weight: 700; }"
        "QLabel#statusLabel { color: #15803D; font-size: 12px; font-weight: 700; }"
        "QLineEdit { background: #FFFFFF; border: 1px solid #CBD5E1; border-radius: 8px; "
        "padding: 10px 12px; color: #0F172A; }"
        "QLineEdit:focus { border: 1px solid #2563EB; }"
        "QPushButton#primaryButton { background: #2563EB; color: #FFFFFF; border: 0; "
        "border-radius: 8px; padding: 10px 16px; font-weight: 700; }"
        "QPushButton#primaryButton:disabled { background: #CBD5E1; color: #64748B; }"
        "QFrame#editorCard { background: #FFFFFF; border: 1px solid #E2E8F0; border-radius: 10px; }"
        "QFrame#progressCard { background: #FFFFFF; border: 1px solid #E2E8F0; border-radius: 10px; }"
        "QLabel#editorCardTitle { color: #0F172A; font-size: 14px; font-weight: 700; }"
        "QLabel#editorSymbol { color: #2563EB; font-size: 18px; font-weight: 700; }"
        "QLabel#editorList { color: #475569; font-size: 13px; }"
        "QLabel#progressTitle, QLabel#fieldLabel { color: #64748B; font-size: 10px; font-weight: 700; }"
        "QLabel#progressText { color: #0F172A; font-size: 12px; font-weight: 600; }"
        "QLabel#editorMessage { color: #15803D; font-size: 12px; font-weight: 600; }"
        "QLabel#editorMessage[error=\"true\"] { color: #DC2626; }"
        "QComboBox { background: #FFFFFF; border: 1px solid #CBD5E1; border-radius: 8px; "
        "padding: 8px 10px; min-height: 20px; color: #0F172A; }"
        "QComboBox:focus { border: 1px solid #2563EB; }"
        "QComboBox:hover { border: 1px solid #94A3B8; }"
        "QComboBox QAbstractItemView { background-color: #FFFFFF; color: #0F172A; "
        "border: 1px solid #CBD5E1; outline: 0; selection-background-color: #DBEAFE; "
        "selection-color: #0F172A; }"
        "QComboBox QAbstractItemView::item { min-height: 32px; padding: 6px 10px; "
        "color: #0F172A; background-color: #FFFFFF; }"
        "QComboBox QAbstractItemView::item:selected { background-color: #DBEAFE; "
        "color: #1E3A8A; }"
        "QPushButton#editorActionButton { background: #2563EB; color: #FFFFFF; border: 0; "
        "border-radius: 8px; padding: 9px 13px; font-weight: 700; }"
        "QPushButton#editorActionButton:hover { background: #1D4ED8; }"
        "QPushButton#editorActionButton:disabled { background: #CBD5E1; color: #64748B; }"
        "QTableWidget { background: #FFFFFF; border: 1px solid #E2E8F0; border-radius: 8px; "
        "gridline-color: #E2E8F0; color: #334155; }"
        "QHeaderView::section { background: #F8FAFC; border: 0; border-bottom: 1px solid #E2E8F0; "
        "padding: 8px; color: #64748B; font-size: 11px; font-weight: 700; }"
        "QFrame#validationCard { background: #FFFFFF; border: 1px solid #E2E8F0; border-radius: 10px; }"
        "QLabel#validationTitle { color: #64748B; font-size: 10px; font-weight: 700; }"
        "QLabel#validationStatus { color: #2563EB; font-size: 15px; font-weight: 700; }"
        "QLabel#validationStatus[valid=\"true\"] { color: #16A34A; }"
        "QLabel#validationStatus[invalid=\"true\"] { color: #DC2626; }"
        "QLabel#validationDetail, QLabel#validationCount { color: #64748B; font-size: 12px; }"
        "QLabel#validationCount { font-weight: 700; }"
        "QPushButton#validationButton { background: #2563EB; color: #FFFFFF; border: 0; "
        "border-radius: 8px; padding: 10px 16px; font-weight: 700; }"
        "QPushButton#validationButton:hover { background: #1D4ED8; }"
        "QPlainTextEdit#errorPanel { background: #FEF2F2; color: #991B1B; "
        "border: 1px solid #FECACA; border-radius: 8px; padding: 8px; }"
        "QFrame#testRequirements, QFrame#testResultCard { background: #FFFFFF; border: 1px solid #E2E8F0; border-radius: 10px; }"
        "QLabel#testSectionTitle, QLabel#testCardTitle { color: #64748B; font-size: 10px; font-weight: 700; }"
        "QLabel#testRequirementName { color: #0F172A; font-size: 13px; font-weight: 600; }"
        "QLabel#testRequirementStatus { color: #64748B; font-size: 13px; }"
        "QLabel#testRequirementStatus[valid=\"true\"] { color: #16A34A; font-weight: 700; }"
        "QLabel#testResultStatus { color: #64748B; font-size: 15px; font-weight: 700; }"
        "QLabel#testResultStatus[error=\"true\"] { color: #DC2626; }"
        "QLabel#testResultStatus[error=\"false\"] { color: #16A34A; }"
        "QLabel#testResultFinal, QLabel#testHint { color: #64748B; font-size: 12px; }"
        "QLabel#testMessage { color: #2563EB; font-size: 13px; font-weight: 600; }"
        "QLabel#testConsistency { color: #16A34A; font-size: 13px; font-weight: 700; }"
        "QLabel#testConsistency[error=\"true\"] { color: #DC2626; }"
        "QLabel#testTraceTitle { color: #0F172A; font-size: 13px; font-weight: 700; }"
        "QPlainTextEdit#testTrace { background: #FFFFFF; border: 1px solid #E2E8F0; "
        "border-radius: 8px; color: #334155; padding: 8px; }"
        "QLabel#editorSectionTitle { color: #64748B; font-size: 10px; font-weight: 700; }"
        "QLabel#testStepIndicator { color: #0F172A; font-size: 13px; font-weight: 700; }"
        "QFrame#unionRequirements, QFrame#unionCard { background: #FFFFFF; border: 1px solid #E2E8F0; border-radius: 10px; }"
        "QLabel#unionSectionTitle, QLabel#unionCardTitle { color: #64748B; font-size: 10px; font-weight: 700; }"
        "QLabel#unionRequirementName { color: #0F172A; font-size: 13px; font-weight: 600; }"
        "QLabel#unionPending { color: #64748B; font-size: 13px; }"
        "QLabel#unionPending[valid=\"true\"] { color: #16A34A; font-weight: 700; }"
        "QLabel#unionMessage { color: #2563EB; font-size: 13px; font-weight: 600; }"
        "QLabel#unionMessage[error=\"true\"] { color: #DC2626; }"
        "QLabel#unionCardText, QLabel#unionLongText { color: #334155; font-size: 13px; }"
        "QTableWidget#unionTable { background: #FFFFFF; border: 1px solid #E2E8F0; "
        "gridline-color: #E2E8F0; color: #334155; }"
    );
}

void MainWindow::crearInterfaz() {
    QWidget* central = new QWidget(this);
    central->setObjectName("central");
    setCentralWidget(central);

    QHBoxLayout* raiz = new QHBoxLayout(central);
    raiz->setContentsMargins(0, 0, 0, 0);
    raiz->setSpacing(0);
    raiz->addWidget(crearSidebar());

    QVBoxLayout* contenidoDerecho = new QVBoxLayout;
    contenidoDerecho->setContentsMargins(0, 0, 0, 0);
    contenidoDerecho->setSpacing(0);

    QFrame* encabezado = new QFrame(central);
    encabezado->setObjectName("header");
    encabezado->setFixedHeight(76);
    QHBoxLayout* encabezadoLayout = new QHBoxLayout(encabezado);
    encabezadoLayout->setContentsMargins(28, 12, 28, 12);
    QVBoxLayout* identidadHeader = new QVBoxLayout;
    identidadHeader->setSpacing(2);
    identidadHeader->addWidget(crearEtiqueta("Sistema de Autómatas DFA", "headerTitle"));
    identidadHeader->addWidget(crearEtiqueta("Teoría de la Computación", "headerSub"));
    encabezadoLayout->addLayout(identidadHeader);
    encabezadoLayout->addStretch();
    encabezadoLayout->addWidget(crearEtiqueta("Qt 6  •  C++17", "headerMeta"));
    contenidoDerecho->addWidget(encabezado);

    paginas = new QStackedWidget(central);
    paginas->addWidget(crearPaginaInicio());
    paginas->addWidget(crearPaginaDFA("DFA 1", "Define los componentes del primer autómata."));
    paginas->addWidget(crearPaginaDFA("DFA 2", "Define los componentes del segundo autómata."));
    paginas->addWidget(crearPaginaUnion());
    paginas->addWidget(crearPaginaPruebas());
    contenidoDerecho->addWidget(paginas, 1);
    raiz->addLayout(contenidoDerecho, 1);
}

QWidget* MainWindow::crearSidebar() {
    QWidget* sidebar = new QWidget;
    sidebar->setObjectName("sidebar");
    sidebar->setFixedWidth(230);
    QVBoxLayout* layout = new QVBoxLayout(sidebar);
    layout->setContentsMargins(18, 24, 18, 20);
    layout->setSpacing(7);
    QHBoxLayout* marca = new QHBoxLayout;
    marca->setSpacing(10);
    marca->addWidget(crearEtiqueta("δ", "brandMark"));
    marca->addWidget(crearEtiqueta("DFA LAB", "brand"));
    marca->addStretch();
    layout->addLayout(marca);
    layout->addWidget(crearEtiqueta("Laboratorio académico", "brandSub"));
    layout->addSpacing(24);
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
    layout->addWidget(crearEtiqueta("Proyecto académico\nC++17 + Qt 6", "brandSub"));
    return sidebar;
}

QWidget* MainWindow::crearPaginaInicio() {
    QWidget* pagina = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(pagina);
    layout->setContentsMargins(32, 28, 32, 28);
    layout->setSpacing(10);
    layout->addWidget(crearEtiqueta("BIENVENIDA", "eyebrow"));
    layout->addWidget(crearEtiqueta("Sistema de Autómatas Finitos Deterministas", "pageTitle"));
    layout->addWidget(crearEtiqueta("Construye, valida y analiza la unión de dos DFA.", "intro"));
    layout->addSpacing(12);
    QFrame* hero = new QFrame;
    hero->setObjectName("hero");
    QVBoxLayout* heroLayout = new QVBoxLayout(hero);
    heroLayout->setContentsMargins(20, 16, 20, 16);
    heroLayout->setSpacing(5);
    heroLayout->addWidget(crearEtiqueta("Crea dos autómatas deterministas", "heroTitle"));
    heroLayout->addWidget(crearEtiqueta("Después valida, genera la unión y prueba cadenas paso a paso.", "heroText"));
    layout->addWidget(hero);
    layout->addSpacing(12);
    layout->addWidget(crearEtiqueta("FLUJO DE TRABAJO", "eyebrow"));
    QHBoxLayout* tarjetas = new QHBoxLayout;
    tarjetas->setSpacing(12);
    tarjetas->addWidget(crearTarjetaFlujo("01", "Definir DFA", "DFA 1 + DFA 2"));
    tarjetas->addWidget(crearTarjetaFlujo("02", "Validar y unir", "Comprueba y construye"));
    tarjetas->addWidget(crearTarjetaFlujo("03", "Analizar cadenas", "Explora los recorridos"));
    layout->addLayout(tarjetas);
    layout->addSpacing(12);
    layout->addWidget(crearEtiqueta("ESTADO DEL PROYECTO", "eyebrow"));
    layout->addWidget(crearEtiqueta("●  Motor lógico preparado   •   Interfaz gráfica activa", "statusLabel"));
    layout->addStretch(1);
    return pagina;
}

QWidget* MainWindow::crearPaginaDFA(const QString& titulo, const QString& subtitulo) {
    QScrollArea* scroll = new QScrollArea;
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    if (titulo == "DFA 1") {
        editorDFA1 = new EditorDFAWidget(dfa1, titulo, subtitulo);
        scroll->setWidget(editorDFA1);
    } else {
        editorDFA2 = new EditorDFAWidget(dfa2, titulo, subtitulo);
        scroll->setWidget(editorDFA2);
    }
    return scroll;
}

QWidget* MainWindow::crearPaginaUnion() {
    QScrollArea* scroll = new QScrollArea;
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    vistaUnionDFA = new VistaUnionDFAWidget(dfa1, dfa2, editorDFA1, editorDFA2);
    scroll->setWidget(vistaUnionDFA);
    return scroll;
}

QWidget* MainWindow::crearPaginaPruebas() {
    QScrollArea* scroll = new QScrollArea;
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    vistaPruebaCadena = new VistaPruebaCadenaWidget(
        dfa1, dfa2, editorDFA1, editorDFA2, vistaUnionDFA);
    scroll->setWidget(vistaPruebaCadena);
    return scroll;
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

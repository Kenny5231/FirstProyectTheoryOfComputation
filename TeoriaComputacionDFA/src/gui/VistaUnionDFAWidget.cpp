#include "gui/VistaUnionDFAWidget.h"

#include "automata/DFA.h"
#include "automata/DFAUnion.h"
#include "estructuras/NodoEstadoCompuesto.h"
#include "estructuras/NodoSimbolo.h"
#include "estructuras/NodoTransicionCompuesta.h"
#include "gui/EditorDFAWidget.h"
#include "operaciones/CompatibilidadDFA.h"
#include "operaciones/ConstructorDFAUnion.h"
#include "validacion/ListaErrores.h"
#include "validacion/NodoError.h"
#include "gui/VisualizadorAutomataWidget.h"

#include <QFrame>
#include <QAbstractItemView>
#include <QGridLayout>
#include <QHeaderView>
#include <QLabel>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>

namespace {
QLabel* etiqueta(const QString& texto, const QString& nombre) {
    QLabel* resultado = new QLabel(texto);
    resultado->setObjectName(nombre);
    resultado->setWordWrap(true);
    return resultado;
}

QFrame* tarjeta(const QString& titulo, QLabel*& contenido) {
    QFrame* marco = new QFrame;
    marco->setObjectName("unionCard");
    QVBoxLayout* layout = new QVBoxLayout(marco);
    layout->setContentsMargins(16, 14, 16, 14);
    layout->setSpacing(5);
    layout->addWidget(etiqueta(titulo, "unionCardTitle"));
    contenido = etiqueta("—", "unionCardText");
    layout->addWidget(contenido);
    return marco;
}

QString formarEstado(const NodoEstadoCompuesto* estado) {
    return "(" + QString::fromStdString(estado->estadoDFA1) + "," +
           QString::fromStdString(estado->estadoDFA2) + ")";
}
}

VistaUnionDFAWidget::VistaUnionDFAWidget(DFA& dfa1Referencia, DFA& dfa2Referencia,
                                         EditorDFAWidget* editor1,
                                         EditorDFAWidget* editor2,
                                         QWidget* parent)
    : QWidget(parent),
      dfa1(&dfa1Referencia),
      dfa2(&dfa2Referencia),
      editorDFA1(editor1),
      editorDFA2(editor2),
      dfaUnion(nullptr),
      estadoDFA1Label(nullptr),
      estadoDFA2Label(nullptr),
      estadoAlfabetosLabel(nullptr),
      mensajeLabel(nullptr),
      resumenEstadosLabel(nullptr),
      resumenSimbolosLabel(nullptr),
      resumenInicialLabel(nullptr),
      resumenFinalesLabel(nullptr),
      resumenTransicionesLabel(nullptr),
      estadosLabel(nullptr),
      simbolosLabel(nullptr),
      inicialLabel(nullptr),
      finalesLabel(nullptr),
      estadoResultadoLabel(nullptr),
      panelErrores(nullptr),
      tablaTransiciones(nullptr),
    botonGenerar(nullptr),
    visualizadorUnion(nullptr) {
    crearInterfaz();
    conectarEventos();
    actualizarDisponibilidad();
}

VistaUnionDFAWidget::~VistaUnionDFAWidget() {
    delete dfaUnion;
    dfaUnion = nullptr;
}

bool VistaUnionDFAWidget::hayUnionGenerada() const {
    return dfaUnion != nullptr;
}

const DFAUnion* VistaUnionDFAWidget::obtenerDFAUnion() const {
    return dfaUnion;
}

void VistaUnionDFAWidget::crearInterfaz() {
    QVBoxLayout* principal = new QVBoxLayout(this);
    principal->setContentsMargins(32, 28, 32, 30);
    principal->setSpacing(12);
    principal->addWidget(etiqueta("OPERACIÓN", "eyebrow"));
    principal->addWidget(etiqueta("DFA Unión", "pageTitle"));
    principal->addWidget(etiqueta("Construye la unión de los dos autómatas validados.", "pageSub"));

    QFrame* requisitos = new QFrame;
    requisitos->setObjectName("unionRequirements");
    QGridLayout* requisitosLayout = new QGridLayout(requisitos);
    requisitosLayout->setContentsMargins(18, 14, 18, 14);
    requisitosLayout->setHorizontalSpacing(22);
    requisitosLayout->setVerticalSpacing(6);
    requisitosLayout->addWidget(etiqueta("REQUISITOS", "unionSectionTitle"), 0, 0, 1, 2);
    requisitosLayout->addWidget(etiqueta("DFA 1", "unionRequirementName"), 1, 0);
    requisitosLayout->addWidget(estadoDFA1Label = etiqueta("Pendiente de validación", "unionPending"), 1, 1);
    requisitosLayout->addWidget(etiqueta("DFA 2", "unionRequirementName"), 2, 0);
    requisitosLayout->addWidget(estadoDFA2Label = etiqueta("Pendiente de validación", "unionPending"), 2, 1);
    requisitosLayout->addWidget(etiqueta("Alfabetos", "unionRequirementName"), 3, 0);
    requisitosLayout->addWidget(estadoAlfabetosLabel = etiqueta("No comprobados", "unionPending"), 3, 1);
    botonGenerar = new QPushButton("Generar DFA Unión");
    botonGenerar->setObjectName("validationButton");
    botonGenerar->setToolTip("Valida correctamente DFA 1 y DFA 2 antes de generar la unión.");
    requisitosLayout->addWidget(botonGenerar, 4, 1);
    principal->addWidget(requisitos);

    mensajeLabel = etiqueta("Valida ambos DFA para habilitar la construcción.", "unionMessage");
    principal->addWidget(mensajeLabel);
    panelErrores = new QPlainTextEdit;
    panelErrores->setObjectName("unionErrorPanel");
    panelErrores->setReadOnly(true);
    panelErrores->setVisible(false);
    panelErrores->setMaximumHeight(140);
    principal->addWidget(panelErrores);

    QGridLayout* resumen = new QGridLayout;
    resumen->setHorizontalSpacing(12);
    resumen->addWidget(tarjeta("Estados", resumenEstadosLabel), 0, 0);
    resumen->addWidget(tarjeta("Símbolos", resumenSimbolosLabel), 0, 1);
    resumen->addWidget(tarjeta("Estado inicial", resumenInicialLabel), 0, 2);
    resumen->addWidget(tarjeta("Estados finales", resumenFinalesLabel), 0, 3);
    resumen->addWidget(tarjeta("Transiciones", resumenTransicionesLabel), 0, 4);
    principal->addLayout(resumen);

    QFrame* estados = new QFrame;
    estados->setObjectName("unionCard");
    QVBoxLayout* estadosLayout = new QVBoxLayout(estados);
    estadosLayout->setContentsMargins(18, 16, 18, 16);
    estadosLayout->addWidget(etiqueta("ESTADOS COMPUESTOS  QU", "unionSectionTitle"));
    estadosLabel = etiqueta("—", "unionLongText");
    estadosLayout->addWidget(estadosLabel);
    principal->addWidget(estados);

    QGridLayout* detalles = new QGridLayout;
    detalles->setHorizontalSpacing(12);
    QFrame* simbolos = tarjeta("Alfabeto ΣU", simbolosLabel);
    QFrame* inicial = tarjeta("Estado inicial q0U", inicialLabel);
    QFrame* finales = tarjeta("Estados finales FU", finalesLabel);
    detalles->addWidget(simbolos, 0, 0);
    detalles->addWidget(inicial, 0, 1);
    detalles->addWidget(finales, 0, 2);
    principal->addLayout(detalles);

    principal->addWidget(etiqueta("VISUALIZACIÓN DEL DFA UNIÓN", "unionSectionTitle"));
    visualizadorUnion = new VisualizadorAutomataWidget;
    principal->addWidget(visualizadorUnion);

    QFrame* tablaMarco = new QFrame;
    tablaMarco->setObjectName("unionCard");
    QVBoxLayout* tablaLayout = new QVBoxLayout(tablaMarco);
    tablaLayout->setContentsMargins(18, 16, 18, 16);
    tablaLayout->addWidget(etiqueta("TABLA DE TRANSICIONES", "unionSectionTitle"));
    tablaTransiciones = new QTableWidget;
    tablaTransiciones->setObjectName("unionTable");
    tablaTransiciones->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tablaTransiciones->verticalHeader()->setVisible(false);
    tablaTransiciones->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tablaTransiciones->setMinimumHeight(180);
    tablaLayout->addWidget(tablaTransiciones);
    principal->addWidget(tablaMarco);
    estadoResultadoLabel = etiqueta("La unión aún no ha sido generada.", "unionMessage");
    principal->addWidget(estadoResultadoLabel);
    principal->addStretch(1);
}

void VistaUnionDFAWidget::conectarEventos() {
    connect(botonGenerar, &QPushButton::clicked, this, [this]() { generarUnion(); });
    connect(editorDFA1, &EditorDFAWidget::estadoValidacionCambiado,
            this, [this](bool) { actualizarDisponibilidad(); });
    connect(editorDFA2, &EditorDFAWidget::estadoValidacionCambiado,
            this, [this](bool) { actualizarDisponibilidad(); });
    connect(editorDFA1, &EditorDFAWidget::dfaModificado,
            this, [this]() { invalidarUnion(); });
    connect(editorDFA2, &EditorDFAWidget::dfaModificado,
            this, [this]() { invalidarUnion(); });
}

void VistaUnionDFAWidget::actualizarDisponibilidad() {
    const bool valido1 = editorDFA1->esDFAValido();
    const bool valido2 = editorDFA2->esDFAValido();
    estadoDFA1Label->setText(valido1 ? "Válido" : (editorDFA1->estaValidado() ? "Inválido" : "Pendiente de validación"));
    estadoDFA2Label->setText(valido2 ? "Válido" : (editorDFA2->estaValidado() ? "Inválido" : "Pendiente de validación"));
    estadoDFA1Label->setProperty("valid", valido1);
    estadoDFA2Label->setProperty("valid", valido2);
    botonGenerar->setEnabled(valido1 && valido2);
    botonGenerar->setToolTip(valido1 && valido2 ? "Genera el DFA Unión." : "Valida correctamente DFA 1 y DFA 2 antes de generar la unión.");
    estadoDFA1Label->style()->unpolish(estadoDFA1Label);
    estadoDFA1Label->style()->polish(estadoDFA1Label);
    estadoDFA2Label->style()->unpolish(estadoDFA2Label);
    estadoDFA2Label->style()->polish(estadoDFA2Label);
}

void VistaUnionDFAWidget::invalidarUnion() {
    if (dfaUnion != nullptr) {
        delete dfaUnion;
        dfaUnion = nullptr;
    }
    limpiarResultado();
    visualizadorUnion->limpiar();
    mensajeLabel->setText("Los autómatas fueron modificados. Valídalos nuevamente y genera una nueva unión.");
    actualizarDisponibilidad();
    emit estadoUnionCambiado(false);
}

void VistaUnionDFAWidget::mostrarError(const QString& mensaje) {
    mensajeLabel->setText(mensaje);
    mensajeLabel->setProperty("error", true);
    mensajeLabel->style()->unpolish(mensajeLabel);
    mensajeLabel->style()->polish(mensajeLabel);
}

void VistaUnionDFAWidget::mostrarEstado(const QString& mensaje, bool correcto) {
    mensajeLabel->setText(mensaje);
    mensajeLabel->setProperty("error", !correcto);
    mensajeLabel->style()->unpolish(mensajeLabel);
    mensajeLabel->style()->polish(mensajeLabel);
}

void VistaUnionDFAWidget::generarUnion() {
    if (!editorDFA1->esDFAValido() || !editorDFA2->esDFAValido()) {
        mostrarError("No se puede generar la unión: ambos DFA deben ser válidos.");
        emit estadoUnionCambiado(false);
        return;
    }
    CompatibilidadDFA compatibilidad;
    ListaErrores erroresCompatibilidad;
    if (!compatibilidad.compararAlfabetos(*dfa1, *dfa2, erroresCompatibilidad)) {
        QString errores;
        const NodoError* actual = erroresCompatibilidad.obtenerPrimero();
        int numero = 1;
        while (actual != nullptr) {
            errores += QString::number(numero) + ". " + QString::fromStdString(actual->mensaje) + "\n";
            actual = actual->siguiente;
            ++numero;
        }
        panelErrores->setPlainText(errores);
        panelErrores->setVisible(true);
        estadoAlfabetosLabel->setText("Incompatibles");
        mostrarError("ALFABETOS INCOMPATIBLES");
        emit estadoUnionCambiado(false);
        return;
    }
    panelErrores->clear();
    panelErrores->setVisible(false);
    estadoAlfabetosLabel->setText("Compatibles");
    DFAUnion* nuevaUnion = new DFAUnion;
    ConstructorDFAUnion constructor;
    if (!constructor.construir(*dfa1, *dfa2, *nuevaUnion)) {
        delete nuevaUnion;
        mostrarError("No fue posible construir el DFA Unión.");
        emit estadoUnionCambiado(false);
        return;
    }
    delete dfaUnion;
    dfaUnion = nuevaUnion;
    visualizadorUnion->mostrarDFAUnion(*dfaUnion);
    mostrarEstado("DFA UNIÓN GENERADO CORRECTAMENTE", true);
    actualizarResultado();
    emit estadoUnionCambiado(true);
}

void VistaUnionDFAWidget::limpiarResultado() {
    resumenEstadosLabel->setText("—");
    resumenSimbolosLabel->setText("—");
    resumenInicialLabel->setText("—");
    resumenFinalesLabel->setText("—");
    resumenTransicionesLabel->setText("—");
    estadosLabel->setText("—");
    simbolosLabel->setText("—");
    inicialLabel->setText("—");
    finalesLabel->setText("—");
    estadoAlfabetosLabel->setText("No comprobados");
    estadoAlfabetosLabel->setProperty("valid", false);
    panelErrores->clear();
    panelErrores->setVisible(false);
    tablaTransiciones->clear();
    tablaTransiciones->setRowCount(0);
    tablaTransiciones->setColumnCount(0);
    estadoResultadoLabel->setText("La unión aún no ha sido generada.");
    estadoAlfabetosLabel->style()->unpolish(estadoAlfabetosLabel);
    estadoAlfabetosLabel->style()->polish(estadoAlfabetosLabel);
}

QString VistaUnionDFAWidget::textoEstados() const {
    QString texto;
    const NodoEstadoCompuesto* actual = dfaUnion->obtenerEstados().obtenerPrimero();
    while (actual != nullptr) {
        if (!texto.isEmpty()) texto += ", ";
        texto += formarEstado(actual);
        actual = actual->siguiente;
    }
    return texto;
}

QString VistaUnionDFAWidget::textoSimbolos() const {
    QString texto;
    const NodoSimbolo* actual = dfaUnion->obtenerAlfabeto().obtenerPrimero();
    while (actual != nullptr) {
        if (!texto.isEmpty()) texto += ", ";
        texto += QString::fromStdString(actual->simbolo);
        actual = actual->siguiente;
    }
    return texto;
}

QString VistaUnionDFAWidget::textoFinales() const {
    QString texto;
    const NodoEstadoCompuesto* actual = dfaUnion->obtenerEstadosFinales().obtenerPrimero();
    while (actual != nullptr) {
        if (!texto.isEmpty()) texto += ", ";
        texto += formarEstado(actual);
        actual = actual->siguiente;
    }
    return texto.isEmpty() ? "F₍U₎ = { }" : texto;
}

QString VistaUnionDFAWidget::textoEstadoInicial() const {
    if (!dfaUnion->tieneEstadoInicial()) return "No definido";
    return "(" + QString::fromStdString(dfaUnion->obtenerEstadoInicialDFA1()) + "," +
           QString::fromStdString(dfaUnion->obtenerEstadoInicialDFA2()) + ")";
}

void VistaUnionDFAWidget::actualizarResultado() {
    resumenEstadosLabel->setText(QString::number(dfaUnion->obtenerEstados().cantidad()));
    resumenSimbolosLabel->setText(QString::number(dfaUnion->obtenerAlfabeto().cantidad()));
    resumenInicialLabel->setText(textoEstadoInicial());
    resumenFinalesLabel->setText(QString::number(dfaUnion->obtenerEstadosFinales().cantidad()));
    resumenTransicionesLabel->setText(QString::number(dfaUnion->obtenerTransiciones().cantidad()));
    estadosLabel->setText(textoEstados());
    simbolosLabel->setText(textoSimbolos());
    inicialLabel->setText(textoEstadoInicial());
    finalesLabel->setText(textoFinales());
    estadoResultadoLabel->setText("La construcción utiliza los DFA validados como fuente de datos.");
    reconstruirTabla();
}

bool VistaUnionDFAWidget::buscarDestino(const std::string& origenDFA1,
                                        const std::string& origenDFA2,
                                        const std::string& simbolo,
                                        std::string& destinoDFA1,
                                        std::string& destinoDFA2) const {
    const NodoTransicionCompuesta* actual = dfaUnion->obtenerTransiciones().obtenerPrimero();
    while (actual != nullptr) {
        if (actual->origenDFA1 == origenDFA1 && actual->origenDFA2 == origenDFA2 &&
            actual->simbolo == simbolo) {
            destinoDFA1 = actual->destinoDFA1;
            destinoDFA2 = actual->destinoDFA2;
            return true;
        }
        actual = actual->siguiente;
    }
    return false;
}

void VistaUnionDFAWidget::reconstruirTabla() {
    const int cantidadSimbolos = dfaUnion->obtenerAlfabeto().cantidad();
    tablaTransiciones->setColumnCount(cantidadSimbolos + 1);
    tablaTransiciones->setRowCount(dfaUnion->obtenerEstados().cantidad());
    tablaTransiciones->setHorizontalHeaderItem(0, new QTableWidgetItem("Estado"));
    const NodoSimbolo* simbolo = dfaUnion->obtenerAlfabeto().obtenerPrimero();
    int columna = 1;
    while (simbolo != nullptr) {
        tablaTransiciones->setHorizontalHeaderItem(columna,
            new QTableWidgetItem(QString::fromStdString(simbolo->simbolo)));
        simbolo = simbolo->siguiente;
        ++columna;
    }
    const NodoEstadoCompuesto* estado = dfaUnion->obtenerEstados().obtenerPrimero();
    int fila = 0;
    while (estado != nullptr) {
        const bool inicial = dfaUnion->tieneEstadoInicial() &&
            estado->estadoDFA1 == dfaUnion->obtenerEstadoInicialDFA1() &&
            estado->estadoDFA2 == dfaUnion->obtenerEstadoInicialDFA2();
        const bool final = dfaUnion->obtenerEstadosFinales().existe(
            estado->estadoDFA1, estado->estadoDFA2);
        QString marca = inicial && final ? "->* " : (inicial ? "-> " : (final ? "* " : ""));
        tablaTransiciones->setItem(fila, 0, new QTableWidgetItem(marca + formarEstado(estado)));
        simbolo = dfaUnion->obtenerAlfabeto().obtenerPrimero();
        columna = 1;
        while (simbolo != nullptr) {
            std::string destinoDFA1;
            std::string destinoDFA2;
            QString destino = "—";
            if (buscarDestino(estado->estadoDFA1, estado->estadoDFA2,
                              simbolo->simbolo, destinoDFA1, destinoDFA2)) {
                destino = "(" + QString::fromStdString(destinoDFA1) + "," +
                          QString::fromStdString(destinoDFA2) + ")";
            }
            tablaTransiciones->setItem(fila, columna, new QTableWidgetItem(destino));
            simbolo = simbolo->siguiente;
            ++columna;
        }
        estado = estado->siguiente;
        ++fila;
    }
}

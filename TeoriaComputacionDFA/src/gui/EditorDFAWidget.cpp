#include "gui/EditorDFAWidget.h"

#include "automata/DFA.h"
#include "estructuras/NodoEstado.h"
#include "estructuras/NodoSimbolo.h"
#include "estructuras/NodoTransicion.h"
#include "validacion/ListaErrores.h"
#include "validacion/NodoError.h"
#include "validacion/ValidadorDFA.h"
#include "gui/VisualizadorAutomataWidget.h"

#include <QAbstractItemView>
#include <QComboBox>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QPlainTextEdit>
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

QPushButton* botonAccion(const QString& texto) {
    QPushButton* boton = new QPushButton(texto);
    boton->setObjectName("editorActionButton");
    boton->setCursor(Qt::PointingHandCursor);
    return boton;
}

QLineEdit* entrada(const QString& placeholder) {
    QLineEdit* campo = new QLineEdit;
    campo->setPlaceholderText(placeholder);
    campo->setMinimumHeight(38);
    return campo;
}

void configurarCombo(QComboBox* combo) {
    combo->view()->setStyleSheet(
        "QAbstractItemView { background-color: #FFFFFF; color: #0F172A; "
        "border: 1px solid #CBD5E1; outline: 0; selection-background-color: #DBEAFE; "
        "selection-color: #0F172A; }"
        "QAbstractItemView::item { min-height: 32px; padding: 6px 10px; "
        "color: #0F172A; background-color: #FFFFFF; }"
        "QAbstractItemView::item:selected { background-color: #DBEAFE; color: #1E3A8A; }");
}

void agregarOpcionesEstados(QComboBox* combo, const DFA& dfa) {
    const NodoEstado* actual = dfa.obtenerEstados().obtenerPrimero();
    while (actual != nullptr) {
        combo->addItem(QString::fromStdString(actual->nombre));
        actual = actual->siguiente;
    }
}
}

EditorDFAWidget::EditorDFAWidget(DFA& dfaReferencia, const QString& titulo,
                                 const QString& descripcion, QWidget* parent)
    : QWidget(parent),
      dfa(&dfaReferencia),
      entradaEstado(nullptr),
      entradaSimbolo(nullptr),
      botonAgregarEstado(nullptr),
      botonAgregarSimbolo(nullptr),
      comboEstadoInicial(nullptr),
      botonEstablecerInicial(nullptr),
      comboEstadoFinal(nullptr),
      botonAgregarFinal(nullptr),
      comboOrigen(nullptr),
      comboSimboloTransicion(nullptr),
      comboDestino(nullptr),
      botonAgregarTransicion(nullptr),
      resumenEstadosLabel(nullptr),
      resumenSimbolosLabel(nullptr),
      resumenInicialLabel(nullptr),
      resumenFinalesLabel(nullptr),
      resumenTransicionesLabel(nullptr),
    inicialActualLabel(nullptr),
      listaEstadosLabel(nullptr),
      listaSimbolosLabel(nullptr),
      listaFinalesLabel(nullptr),
      mensajeEstado(nullptr),
    tablaTransiciones(nullptr),
        visualizador(nullptr),
    tarjetaValidacion(nullptr),
    estadoValidacionLabel(nullptr),
    detalleValidacionLabel(nullptr),
    cantidadErroresLabel(nullptr),
    panelErrores(nullptr),
    botonValidar(nullptr),
    validacionRealizada(false),
    dfaValido(false) {
    crearInterfaz(titulo, descripcion);
    conectarEventos();
    refrescarInterfaz();
}

QFrame* EditorDFAWidget::crearSeccion(const QString& titulo, const QString& simbolo) {
    QFrame* seccion = new QFrame;
    seccion->setObjectName("editorCard");
    QVBoxLayout* layout = new QVBoxLayout(seccion);
    layout->setContentsMargins(18, 16, 18, 16);
    layout->setSpacing(10);
    QHBoxLayout* encabezado = new QHBoxLayout;
    encabezado->addWidget(etiqueta(titulo, "editorCardTitle"));
    encabezado->addStretch();
    encabezado->addWidget(etiqueta(simbolo, "editorSymbol"));
    layout->addLayout(encabezado);
    return seccion;
}

void EditorDFAWidget::crearInterfaz(const QString& titulo, const QString& descripcion) {
    QVBoxLayout* principal = new QVBoxLayout(this);
    principal->setContentsMargins(32, 28, 32, 30);
    principal->setSpacing(12);
    principal->addWidget(etiqueta("CONFIGURACIÓN", "eyebrow"));
    principal->addWidget(etiqueta(titulo, "pageTitle"));
    principal->addWidget(etiqueta(descripcion, "pageSub"));

    QFrame* progreso = new QFrame;
    progreso->setObjectName("progressCard");
    QGridLayout* progresoLayout = new QGridLayout(progreso);
    progresoLayout->setContentsMargins(18, 14, 18, 14);
    progresoLayout->setHorizontalSpacing(28);
    progresoLayout->setVerticalSpacing(4);
    progresoLayout->addWidget(etiqueta("PROGRESO DEL AUTÓMATA", "progressTitle"), 0, 0, 1, 5);
    resumenEstadosLabel = etiqueta("Estados: 0", "progressText");
    resumenSimbolosLabel = etiqueta("Símbolos: 0", "progressText");
    resumenInicialLabel = etiqueta("Inicial: No definido", "progressText");
    resumenFinalesLabel = etiqueta("Finales: 0", "progressText");
    resumenTransicionesLabel = etiqueta("Transiciones: 0", "progressText");
    progresoLayout->addWidget(resumenEstadosLabel, 1, 0);
    progresoLayout->addWidget(resumenSimbolosLabel, 1, 1);
    progresoLayout->addWidget(resumenInicialLabel, 1, 2);
    progresoLayout->addWidget(resumenFinalesLabel, 1, 3);
    progresoLayout->addWidget(resumenTransicionesLabel, 1, 4);
    principal->addWidget(progreso);

    principal->addWidget(etiqueta("VISUALIZACIÓN DEL AUTÓMATA", "editorSectionTitle"));
    visualizador = new VisualizadorAutomataWidget;
    principal->addWidget(visualizador);

    tarjetaValidacion = new QFrame;
    tarjetaValidacion->setObjectName("validationCard");
    QVBoxLayout* validacionLayout = new QVBoxLayout(tarjetaValidacion);
    validacionLayout->setContentsMargins(18, 14, 18, 14);
    validacionLayout->setSpacing(6);
    validacionLayout->addWidget(etiqueta("VALIDACIÓN", "validationTitle"));
    QHBoxLayout* estadoLayout = new QHBoxLayout;
    estadoValidacionLabel = etiqueta("●  Pendiente de validación", "validationStatus");
    botonValidar = botonAccion("Validar DFA");
    botonValidar->setObjectName("validationButton");
    estadoLayout->addWidget(estadoValidacionLabel, 1);
    estadoLayout->addWidget(botonValidar);
    validacionLayout->addLayout(estadoLayout);
    detalleValidacionLabel = etiqueta("Ejecuta ValidadorDFA para comprobar el autómata.", "validationDetail");
    cantidadErroresLabel = etiqueta("", "validationCount");
    validacionLayout->addWidget(detalleValidacionLabel);
    validacionLayout->addWidget(cantidadErroresLabel);
    panelErrores = new QPlainTextEdit;
    panelErrores->setObjectName("errorPanel");
    panelErrores->setReadOnly(true);
    panelErrores->setVisible(false);
    panelErrores->setMaximumHeight(150);
    validacionLayout->addWidget(panelErrores);
    principal->addWidget(tarjetaValidacion);

    QGridLayout* rejilla = new QGridLayout;
    rejilla->setHorizontalSpacing(12);
    rejilla->setVerticalSpacing(12);

    QFrame* estados = crearSeccion("Estados", "Q");
    QHBoxLayout* estadosEntrada = new QHBoxLayout;
    entradaEstado = entrada("Ejemplo: q0");
    botonAgregarEstado = botonAccion("Agregar");
    estadosEntrada->addWidget(entradaEstado, 1);
    estadosEntrada->addWidget(botonAgregarEstado);
    listaEstadosLabel = etiqueta("Sin estados", "editorList");
    QVBoxLayout* layoutEstados = qobject_cast<QVBoxLayout*>(estados->layout());
    if (layoutEstados != nullptr) {
        layoutEstados->addLayout(estadosEntrada);
        layoutEstados->addWidget(listaEstadosLabel);
    }
    rejilla->addWidget(estados, 0, 0);

    QFrame* simbolos = crearSeccion("Alfabeto", "Σ");
    QHBoxLayout* simbolosEntrada = new QHBoxLayout;
    entradaSimbolo = entrada("Ejemplo: a");
    botonAgregarSimbolo = botonAccion("Agregar");
    simbolosEntrada->addWidget(entradaSimbolo, 1);
    simbolosEntrada->addWidget(botonAgregarSimbolo);
    listaSimbolosLabel = etiqueta("Sin símbolos", "editorList");
    QVBoxLayout* layoutSimbolos = qobject_cast<QVBoxLayout*>(simbolos->layout());
    if (layoutSimbolos != nullptr) {
        layoutSimbolos->addLayout(simbolosEntrada);
        layoutSimbolos->addWidget(listaSimbolosLabel);
    }
    rejilla->addWidget(simbolos, 0, 1);

    QFrame* inicial = crearSeccion("Estado inicial", "q₀");
    QHBoxLayout* inicialEntrada = new QHBoxLayout;
    comboEstadoInicial = new QComboBox;
    configurarCombo(comboEstadoInicial);
    botonEstablecerInicial = botonAccion("Establecer");
    inicialEntrada->addWidget(comboEstadoInicial, 1);
    inicialEntrada->addWidget(botonEstablecerInicial);
    inicialEntrada->setContentsMargins(0, 0, 0, 0);
    inicialActualLabel = etiqueta("Actual: No definido", "editorList");
    inicialActualLabel->setObjectName("initialSummary");
    QVBoxLayout* layoutInicial = qobject_cast<QVBoxLayout*>(inicial->layout());
    if (layoutInicial != nullptr) {
        layoutInicial->addLayout(inicialEntrada);
        layoutInicial->addWidget(inicialActualLabel);
    }
    rejilla->addWidget(inicial, 1, 0);

    QFrame* finales = crearSeccion("Estados finales", "F");
    QHBoxLayout* finalesEntrada = new QHBoxLayout;
    comboEstadoFinal = new QComboBox;
    configurarCombo(comboEstadoFinal);
    botonAgregarFinal = botonAccion("Agregar");
    finalesEntrada->addWidget(comboEstadoFinal, 1);
    finalesEntrada->addWidget(botonAgregarFinal);
    listaFinalesLabel = etiqueta("F = { }", "editorList");
    QVBoxLayout* layoutFinales = qobject_cast<QVBoxLayout*>(finales->layout());
    if (layoutFinales != nullptr) {
        layoutFinales->addLayout(finalesEntrada);
        layoutFinales->addWidget(listaFinalesLabel);
    }
    rejilla->addWidget(finales, 1, 1);
    principal->addLayout(rejilla);

    QFrame* transiciones = crearSeccion("Transiciones", "δ");
    QGridLayout* transicionesLayout = new QGridLayout;
    transicionesLayout->setHorizontalSpacing(8);
    transicionesLayout->addWidget(etiqueta("Origen", "fieldLabel"), 0, 0);
    transicionesLayout->addWidget(etiqueta("Símbolo", "fieldLabel"), 0, 1);
    transicionesLayout->addWidget(etiqueta("Destino", "fieldLabel"), 0, 2);
    comboOrigen = new QComboBox;
    comboSimboloTransicion = new QComboBox;
    comboDestino = new QComboBox;
    configurarCombo(comboOrigen);
    configurarCombo(comboSimboloTransicion);
    configurarCombo(comboDestino);
    botonAgregarTransicion = botonAccion("Agregar transición");
    transicionesLayout->addWidget(comboOrigen, 1, 0);
    transicionesLayout->addWidget(comboSimboloTransicion, 1, 1);
    transicionesLayout->addWidget(comboDestino, 1, 2);
    transicionesLayout->addWidget(botonAgregarTransicion, 1, 3);
    QVBoxLayout* layoutTransiciones = qobject_cast<QVBoxLayout*>(transiciones->layout());
    if (layoutTransiciones != nullptr) {
        layoutTransiciones->addLayout(transicionesLayout);
    }
    tablaTransiciones = new QTableWidget;
    tablaTransiciones->setColumnCount(4);
    tablaTransiciones->setHorizontalHeaderItem(0, new QTableWidgetItem("Origen"));
    tablaTransiciones->setHorizontalHeaderItem(1, new QTableWidgetItem("Símbolo"));
    tablaTransiciones->setHorizontalHeaderItem(2, new QTableWidgetItem("Destino"));
    tablaTransiciones->setHorizontalHeaderItem(3, new QTableWidgetItem("Eliminar"));
    tablaTransiciones->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tablaTransiciones->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
    tablaTransiciones->setColumnWidth(3, 92);
    tablaTransiciones->verticalHeader()->setVisible(false);
    tablaTransiciones->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tablaTransiciones->setSelectionMode(QAbstractItemView::NoSelection);
    tablaTransiciones->setMinimumHeight(130);
    if (layoutTransiciones != nullptr) {
        layoutTransiciones->addWidget(tablaTransiciones);
    }
    principal->addWidget(transiciones);

    mensajeEstado = etiqueta("Autómata en construcción", "editorMessage");
    principal->addWidget(mensajeEstado);
    principal->addStretch(1);
}

void EditorDFAWidget::conectarEventos() {
    connect(botonAgregarEstado, &QPushButton::clicked, this, [this]() { agregarEstado(); });
    connect(botonAgregarSimbolo, &QPushButton::clicked, this, [this]() { agregarSimbolo(); });
    connect(botonEstablecerInicial, &QPushButton::clicked, this, [this]() { establecerInicial(); });
    connect(botonAgregarFinal, &QPushButton::clicked, this, [this]() { agregarFinal(); });
    connect(botonAgregarTransicion, &QPushButton::clicked, this, [this]() { agregarTransicion(); });
    connect(botonValidar, &QPushButton::clicked, this, [this]() { validarDFA(); });
}

bool EditorDFAWidget::estaValidado() const {
    return validacionRealizada;
}

bool EditorDFAWidget::esDFAValido() const {
    return validacionRealizada && dfaValido;
}

void EditorDFAWidget::eliminarTransicion(const QString& origen, const QString& simbolo,
                                         const QString& destino) {
    if (dfa->eliminarTransicion(origen.toStdString(), simbolo.toStdString(),
                                destino.toStdString())) {
        invalidarValidacion();
        mostrarMensaje(QString("Transición %1 --%2--> %3 eliminada.").arg(origen, simbolo, destino),
                       false);
        refrescarInterfaz();
    } else {
        mostrarMensaje("No fue posible eliminar la transición.", true);
    }
}

void EditorDFAWidget::validarDFA() {
    ListaErrores errores;
    ValidadorDFA validador;
    dfaValido = validador.validar(*dfa, errores);
    validacionRealizada = true;

    estadoValidacionLabel->setProperty("valid", dfaValido);
    estadoValidacionLabel->setProperty("invalid", !dfaValido);
    estadoValidacionLabel->setText(dfaValido ? "✓  DFA VÁLIDO" : "✕  DFA INVÁLIDO");
    detalleValidacionLabel->setText(
        dfaValido ? "El autómata cumple todas las condiciones de un DFA determinista."
                  : "Corrige los errores indicados antes de continuar.");
    cantidadErroresLabel->setText(QString("Cantidad de errores: %1").arg(errores.cantidad()));

    if (dfaValido) {
        panelErrores->clear();
        panelErrores->setVisible(false);
    } else {
        QString textoErrores;
        const NodoError* actual = errores.obtenerPrimero();
        int numero = 1;
        while (actual != nullptr) {
            textoErrores += QString::number(numero) + ". "
                            + QString::fromStdString(actual->mensaje) + "\n";
            actual = actual->siguiente;
            ++numero;
        }
        panelErrores->setPlainText(textoErrores);
        panelErrores->setVisible(true);
    }
    estadoValidacionLabel->style()->unpolish(estadoValidacionLabel);
    estadoValidacionLabel->style()->polish(estadoValidacionLabel);
    mensajeEstado->setText(dfaValido ? "DFA válido" : "DFA inválido");
    mensajeEstado->setProperty("error", !dfaValido);
    mensajeEstado->style()->unpolish(mensajeEstado);
    mensajeEstado->style()->polish(mensajeEstado);
    emit estadoValidacionCambiado(dfaValido);
}

void EditorDFAWidget::invalidarValidacion() {
    validacionRealizada = false;
    dfaValido = false;
    estadoValidacionLabel->setProperty("valid", false);
    estadoValidacionLabel->setProperty("invalid", false);
    estadoValidacionLabel->setText("●  Pendiente de validación");
    detalleValidacionLabel->setText("Ejecuta ValidadorDFA para comprobar el autómata.");
    cantidadErroresLabel->clear();
    panelErrores->clear();
    panelErrores->setVisible(false);
    mensajeEstado->setText("Pendiente de validación");
    mensajeEstado->setProperty("error", false);
    estadoValidacionLabel->style()->unpolish(estadoValidacionLabel);
    estadoValidacionLabel->style()->polish(estadoValidacionLabel);
    mensajeEstado->style()->unpolish(mensajeEstado);
    mensajeEstado->style()->polish(mensajeEstado);
    emit estadoValidacionCambiado(false);
    emit dfaModificado();
}

QString EditorDFAWidget::resumenEstados() const {
    QString resultado;
    const NodoEstado* actual = dfa->obtenerEstados().obtenerPrimero();
    while (actual != nullptr) {
        if (!resultado.isEmpty()) resultado += ", ";
        resultado += QString::fromStdString(actual->nombre);
        actual = actual->siguiente;
    }
    return resultado.isEmpty() ? "Sin estados" : resultado;
}

QString EditorDFAWidget::resumenSimbolos() const {
    QString resultado;
    const NodoSimbolo* actual = dfa->obtenerAlfabeto().obtenerPrimero();
    while (actual != nullptr) {
        if (!resultado.isEmpty()) resultado += ", ";
        resultado += QString::fromStdString(actual->simbolo);
        actual = actual->siguiente;
    }
    return resultado.isEmpty() ? "Sin símbolos" : resultado;
}

QString EditorDFAWidget::resumenFinales() const {
    QString resultado;
    const NodoEstado* actual = dfa->obtenerEstadosFinales().obtenerPrimero();
    while (actual != nullptr) {
        if (!resultado.isEmpty()) resultado += ", ";
        resultado += QString::fromStdString(actual->nombre);
        actual = actual->siguiente;
    }
    return resultado.isEmpty() ? "F = { }" : resultado;
}

void EditorDFAWidget::refrescarCombos() {
    comboEstadoInicial->clear();
    comboEstadoFinal->clear();
    comboOrigen->clear();
    comboDestino->clear();
    comboSimboloTransicion->clear();
    agregarOpcionesEstados(comboEstadoInicial, *dfa);
    agregarOpcionesEstados(comboEstadoFinal, *dfa);
    agregarOpcionesEstados(comboOrigen, *dfa);
    agregarOpcionesEstados(comboDestino, *dfa);
    const NodoSimbolo* simbolo = dfa->obtenerAlfabeto().obtenerPrimero();
    while (simbolo != nullptr) {
        comboSimboloTransicion->addItem(QString::fromStdString(simbolo->simbolo));
        simbolo = simbolo->siguiente;
    }
}

void EditorDFAWidget::refrescarTabla() {
    tablaTransiciones->setRowCount(dfa->obtenerTransiciones().cantidad());
    const NodoTransicion* actual = dfa->obtenerTransiciones().obtenerPrimero();
    int fila = 0;
    while (actual != nullptr) {
        const QString origen = QString::fromStdString(actual->origen);
        const QString simbolo = QString::fromStdString(actual->simbolo);
        const QString destino = QString::fromStdString(actual->destino);
        tablaTransiciones->setItem(fila, 0, new QTableWidgetItem(origen));
        tablaTransiciones->setItem(fila, 1, new QTableWidgetItem(simbolo));
        tablaTransiciones->setItem(fila, 2, new QTableWidgetItem(destino));
        QPushButton* botonEliminar = new QPushButton("Eliminar");
        botonEliminar->setObjectName("editorActionButton");
        botonEliminar->setCursor(Qt::PointingHandCursor);
        botonEliminar->setToolTip(QString("Eliminar %1 --%2--> %3").arg(origen, simbolo, destino));
        connect(botonEliminar, &QPushButton::clicked, this,
                [this, origen, simbolo, destino]() { eliminarTransicion(origen, simbolo, destino); });
        tablaTransiciones->setCellWidget(fila, 3, botonEliminar);
        actual = actual->siguiente;
        ++fila;
    }
}

void EditorDFAWidget::refrescarInterfaz() {
    resumenEstadosLabel->setText(QString("Estados: %1").arg(dfa->obtenerEstados().cantidad()));
    resumenSimbolosLabel->setText(QString("Símbolos: %1").arg(dfa->obtenerAlfabeto().cantidad()));
    resumenInicialLabel->setText(dfa->tieneEstadoInicial()
                                     ? QString("Inicial: %1").arg(QString::fromStdString(dfa->obtenerEstadoInicial()))
                                     : "Inicial: No definido");
    resumenFinalesLabel->setText(QString("Finales: %1").arg(dfa->obtenerEstadosFinales().cantidad()));
    resumenTransicionesLabel->setText(QString("Transiciones: %1").arg(dfa->obtenerTransiciones().cantidad()));
    listaEstadosLabel->setText(resumenEstados());
    listaSimbolosLabel->setText(resumenSimbolos());
    listaFinalesLabel->setText(resumenFinales());
    inicialActualLabel->setText(dfa->tieneEstadoInicial()
                                    ? QString("Actual: %1").arg(QString::fromStdString(dfa->obtenerEstadoInicial()))
                                    : "Actual: No definido");
    refrescarCombos();
    refrescarTabla();
    const bool hayEstados = dfa->obtenerEstados().cantidad() > 0;
    const bool haySimbolos = dfa->obtenerAlfabeto().cantidad() > 0;
    botonEstablecerInicial->setEnabled(hayEstados);
    botonAgregarFinal->setEnabled(hayEstados);
    botonAgregarTransicion->setEnabled(hayEstados && haySimbolos);
    visualizador->mostrarDFA(*dfa);
}

void EditorDFAWidget::mostrarMensaje(const QString& texto, bool error) {
    mensajeEstado->setText(texto);
    mensajeEstado->setProperty("error", error);
    mensajeEstado->style()->unpolish(mensajeEstado);
    mensajeEstado->style()->polish(mensajeEstado);
}

void EditorDFAWidget::agregarEstado() {
    const QString texto = entradaEstado->text().trimmed();
    if (texto.isEmpty()) {
        mostrarMensaje("Escribe un estado antes de agregarlo.", true);
        return;
    }
    if (dfa->agregarEstado(texto.toStdString())) {
        invalidarValidacion();
        mostrarMensaje(QString("Estado '%1' agregado.").arg(texto), false);
        entradaEstado->clear();
        refrescarInterfaz();
    } else {
        mostrarMensaje(QString("El estado '%1' ya existe.").arg(texto), true);
    }
}

void EditorDFAWidget::agregarSimbolo() {
    const QString texto = entradaSimbolo->text();
    if (texto.isEmpty()) {
        mostrarMensaje("Escribe un símbolo antes de agregarlo.", true);
        return;
    }
    if (dfa->agregarSimbolo(texto.toStdString())) {
        invalidarValidacion();
        mostrarMensaje(QString("Símbolo '%1' agregado.").arg(texto), false);
        entradaSimbolo->clear();
        refrescarInterfaz();
    } else {
        mostrarMensaje(QString("El símbolo '%1' ya existe.").arg(texto), true);
    }
}

void EditorDFAWidget::establecerInicial() {
    if (comboEstadoInicial->count() == 0) {
        mostrarMensaje("Agrega estados antes de definir q₀.", true);
        return;
    }
    const QString texto = comboEstadoInicial->currentText();
    dfa->establecerEstadoInicial(texto.toStdString());
    invalidarValidacion();
    mostrarMensaje(QString("Estado inicial '%1' establecido.").arg(texto), false);
    refrescarInterfaz();
}

void EditorDFAWidget::agregarFinal() {
    if (comboEstadoFinal->count() == 0) {
        mostrarMensaje("Agrega estados antes de definir F.", true);
        return;
    }
    const QString texto = comboEstadoFinal->currentText();
    if (dfa->agregarEstadoFinal(texto.toStdString())) {
        invalidarValidacion();
        mostrarMensaje(QString("Estado '%1' agregado a F.").arg(texto), false);
        refrescarInterfaz();
    } else {
        mostrarMensaje(QString("El estado '%1' ya pertenece a F.").arg(texto), true);
    }
}

void EditorDFAWidget::agregarTransicion() {
    if (comboOrigen->count() == 0 || comboSimboloTransicion->count() == 0) {
        mostrarMensaje("Agrega estados y símbolos antes de crear transiciones.", true);
        return;
    }
    const QString origen = comboOrigen->currentText();
    const QString simbolo = comboSimboloTransicion->currentText();
    const QString destino = comboDestino->currentText();
    if (dfa->agregarTransicion(origen.toStdString(), simbolo.toStdString(), destino.toStdString())) {
        invalidarValidacion();
        mostrarMensaje(QString("Transición %1 --%2--> %3 agregada.").arg(origen, simbolo, destino), false);
        refrescarInterfaz();
    } else {
        mostrarMensaje("No fue posible agregar la transición.", true);
    }
}

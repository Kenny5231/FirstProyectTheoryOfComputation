#include "gui/VistaPruebaCadenaWidget.h"

#include "automata/DFA.h"
#include "automata/DFAUnion.h"
#include "estructuras/CadenaEntrada.h"
#include "estructuras/ListaPasosDFA.h"
#include "estructuras/ListaPasosDFAUnion.h"
#include "estructuras/NodoPasoDFA.h"
#include "estructuras/NodoPasoDFAUnion.h"
#include "gui/EditorDFAWidget.h"
#include "gui/VistaUnionDFAWidget.h"
#include "simulacion/EvaluadorTriple.h"
#include "simulacion/ResultadoTriple.h"
#include "simulacion/SimuladorDFA.h"
#include "simulacion/SimuladorDFAUnion.h"

#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QStyle>
#include <QVBoxLayout>

namespace {
QLabel* etiqueta(const QString& texto, const QString& nombre) {
    QLabel* resultado = new QLabel(texto);
    resultado->setObjectName(nombre);
    resultado->setWordWrap(true);
    return resultado;
}

QFrame* tarjetaResultado(const QString& titulo, QLabel*& estado, QLabel*& final) {
    QFrame* tarjeta = new QFrame;
    tarjeta->setObjectName("testResultCard");
    QVBoxLayout* layout = new QVBoxLayout(tarjeta);
    layout->setContentsMargins(16, 14, 16, 14);
    layout->setSpacing(6);
    layout->addWidget(etiqueta(titulo, "testCardTitle"));
    estado = etiqueta("Sin evaluar", "testResultStatus");
    final = etiqueta("Estado: —", "testResultFinal");
    layout->addWidget(estado);
    layout->addWidget(final);
    return tarjeta;
}

QString estadoResultado(bool aceptada) {
    return aceptada ? "ACEPTADA" : "RECHAZADA";
}
}

VistaPruebaCadenaWidget::VistaPruebaCadenaWidget(
    DFA& dfa1Referencia, DFA& dfa2Referencia, EditorDFAWidget* editor1,
    EditorDFAWidget* editor2, VistaUnionDFAWidget* vistaUnionReferencia,
    QWidget* parent)
    : QWidget(parent),
      dfa1(&dfa1Referencia),
      dfa2(&dfa2Referencia),
      editorDFA1(editor1),
      editorDFA2(editor2),
      vistaUnion(vistaUnionReferencia),
      entradaCadena(nullptr),
      botonEvaluar(nullptr),
      requisitoDFA1(nullptr),
      requisitoDFA2(nullptr),
      requisitoUnion(nullptr),
      mensajeEstado(nullptr),
      resultadoDFA1(nullptr),
      resultadoDFA2(nullptr),
      resultadoUnion(nullptr),
      estadoFinalDFA1Label(nullptr),
      estadoFinalDFA2Label(nullptr),
      estadoFinalUnionLabel(nullptr),
      consistenciaLabel(nullptr),
      trazaDFA1(nullptr),
      trazaDFA2(nullptr),
      trazaUnion(nullptr),
      hayResultados(false) {
    crearInterfaz();
    conectarEventos();
    actualizarDisponibilidad();
}

void VistaPruebaCadenaWidget::crearInterfaz() {
    QVBoxLayout* principal = new QVBoxLayout(this);
    principal->setContentsMargins(32, 28, 32, 30);
    principal->setSpacing(12);
    principal->addWidget(etiqueta("SIMULACIÓN", "eyebrow"));
    principal->addWidget(etiqueta("Prueba de cadenas", "pageTitle"));
    principal->addWidget(etiqueta("Evalúa una secuencia sobre DFA 1, DFA 2 y el DFA Unión.", "pageSub"));

    QFrame* requisitos = new QFrame;
    requisitos->setObjectName("testRequirements");
    QGridLayout* requisitosLayout = new QGridLayout(requisitos);
    requisitosLayout->setContentsMargins(18, 14, 18, 14);
    requisitosLayout->setHorizontalSpacing(20);
    requisitosLayout->setVerticalSpacing(6);
    requisitosLayout->addWidget(etiqueta("REQUISITOS", "testSectionTitle"), 0, 0, 1, 2);
    requisitosLayout->addWidget(etiqueta("DFA 1", "testRequirementName"), 1, 0);
    requisitosLayout->addWidget(requisitoDFA1 = etiqueta("Pendiente", "testRequirementStatus"), 1, 1);
    requisitosLayout->addWidget(etiqueta("DFA 2", "testRequirementName"), 2, 0);
    requisitosLayout->addWidget(requisitoDFA2 = etiqueta("Pendiente", "testRequirementStatus"), 2, 1);
    requisitosLayout->addWidget(etiqueta("DFA Unión", "testRequirementName"), 3, 0);
    requisitosLayout->addWidget(requisitoUnion = etiqueta("No generado", "testRequirementStatus"), 3, 1);
    principal->addWidget(requisitos);

    principal->addWidget(etiqueta("CADENA DE ENTRADA", "testSectionTitle"));
    entradaCadena = new QLineEdit;
    entradaCadena->setPlaceholderText("Ejemplo: a b a b");
    entradaCadena->setMinimumHeight(40);
    principal->addWidget(entradaCadena);
    principal->addWidget(etiqueta("Separa cada símbolo con espacios. Deja vacío para ε.", "testHint"));
    botonEvaluar = new QPushButton("Evaluar cadena");
    botonEvaluar->setObjectName("validationButton");
    botonEvaluar->setCursor(Qt::PointingHandCursor);
    principal->addWidget(botonEvaluar, 0, Qt::AlignRight);
    mensajeEstado = etiqueta("Valida ambos DFA y genera la unión para habilitar la evaluación.", "testMessage");
    principal->addWidget(mensajeEstado);

    QGridLayout* resultados = new QGridLayout;
    resultados->setHorizontalSpacing(12);
    resultados->addWidget(tarjetaResultado("DFA 1", resultadoDFA1, estadoFinalDFA1Label), 0, 0);
    resultados->addWidget(tarjetaResultado("DFA 2", resultadoDFA2, estadoFinalDFA2Label), 0, 1);
    resultados->addWidget(tarjetaResultado("DFA Unión", resultadoUnion, estadoFinalUnionLabel), 0, 2);
    principal->addLayout(resultados);
    consistenciaLabel = etiqueta("DFA Unión = DFA1 OR DFA2\nPendiente de evaluación", "testConsistency");
    principal->addWidget(consistenciaLabel);

    principal->addWidget(etiqueta("TRAZABILIDAD", "testSectionTitle"));
    QGridLayout* trazas = new QGridLayout;
    trazas->setHorizontalSpacing(12);
    trazaDFA1 = new QPlainTextEdit;
    trazaDFA2 = new QPlainTextEdit;
    trazaUnion = new QPlainTextEdit;
    trazaDFA1->setReadOnly(true);
    trazaDFA2->setReadOnly(true);
    trazaUnion->setReadOnly(true);
    trazaDFA1->setObjectName("testTrace");
    trazaDFA2->setObjectName("testTrace");
    trazaUnion->setObjectName("testTrace");
    trazaDFA1->setPlaceholderText("La traza de DFA 1 aparecerá aquí.");
    trazaDFA2->setPlaceholderText("La traza de DFA 2 aparecerá aquí.");
    trazaUnion->setPlaceholderText("La traza del DFA Unión aparecerá aquí.");
    trazas->addWidget(etiqueta("Traza DFA 1", "testTraceTitle"), 0, 0);
    trazas->addWidget(etiqueta("Traza DFA 2", "testTraceTitle"), 0, 1);
    trazas->addWidget(trazaDFA1, 1, 0);
    trazas->addWidget(trazaDFA2, 1, 1);
    trazas->addWidget(etiqueta("Traza DFA Unión", "testTraceTitle"), 2, 0, 1, 2);
    trazas->addWidget(trazaUnion, 3, 0, 1, 2);
    principal->addLayout(trazas);
    principal->addStretch(1);
}

void VistaPruebaCadenaWidget::conectarEventos() {
    connect(botonEvaluar, &QPushButton::clicked, this, [this]() { evaluarCadena(); });
    connect(editorDFA1, &EditorDFAWidget::dfaModificado, this, [this]() { invalidarResultados(); });
    connect(editorDFA2, &EditorDFAWidget::dfaModificado, this, [this]() { invalidarResultados(); });
    connect(editorDFA1, &EditorDFAWidget::estadoValidacionCambiado,
            this, [this](bool) { actualizarDisponibilidad(); });
    connect(editorDFA2, &EditorDFAWidget::estadoValidacionCambiado,
            this, [this](bool) { actualizarDisponibilidad(); });
    connect(vistaUnion, &VistaUnionDFAWidget::estadoUnionCambiado,
            this, [this](bool generada) {
                if (!generada) invalidarResultados();
                else actualizarDisponibilidad();
            });
}

void VistaPruebaCadenaWidget::actualizarDisponibilidad() {
    const bool valido1 = editorDFA1->esDFAValido();
    const bool valido2 = editorDFA2->esDFAValido();
    const bool unionGenerada = vistaUnion->hayUnionGenerada();
    requisitoDFA1->setText(valido1 ? "Válido" : (editorDFA1->estaValidado() ? "Inválido" : "Pendiente"));
    requisitoDFA2->setText(valido2 ? "Válido" : (editorDFA2->estaValidado() ? "Inválido" : "Pendiente"));
    requisitoUnion->setText(unionGenerada ? "Generado" : "No generado");
    requisitoDFA1->setProperty("valid", valido1);
    requisitoDFA2->setProperty("valid", valido2);
    requisitoUnion->setProperty("valid", unionGenerada);
    const bool disponible = valido1 && valido2 && unionGenerada;
    botonEvaluar->setEnabled(disponible);
    botonEvaluar->setToolTip(disponible ? "Evalúa la cadena." : "Valida ambos DFA y genera la unión antes de evaluar.");
    if (disponible) {
        mensajeEstado->setText("Todo listo. Ingresa una cadena para evaluarla.");
    } else if (!valido1 && !valido2) {
        mensajeEstado->setText("Valida DFA 1 y DFA 2 antes de continuar.");
    } else if (!valido1) {
        mensajeEstado->setText("Valida DFA 1 antes de evaluar cadenas.");
    } else if (!valido2) {
        mensajeEstado->setText("Valida DFA 2 antes de evaluar cadenas.");
    } else {
        mensajeEstado->setText("Genera el DFA Unión antes de evaluar cadenas.");
    }
    requisitoDFA1->style()->unpolish(requisitoDFA1);
    requisitoDFA1->style()->polish(requisitoDFA1);
    requisitoDFA2->style()->unpolish(requisitoDFA2);
    requisitoDFA2->style()->polish(requisitoDFA2);
    requisitoUnion->style()->unpolish(requisitoUnion);
    requisitoUnion->style()->polish(requisitoUnion);
}

void VistaPruebaCadenaWidget::construirCadenaEntrada(const QString& texto, CadenaEntrada& cadena) const {
    cadena.limpiar();
    QString tokenActual;
    for (int indice = 0; indice < texto.size(); ++indice) {
        const QChar caracter = texto.at(indice);
        if (caracter.isSpace()) {
            if (!tokenActual.isEmpty()) {
                cadena.agregarSimbolo(tokenActual.toStdString());
                tokenActual.clear();
            }
        } else {
            tokenActual += caracter;
        }
    }
    if (!tokenActual.isEmpty()) {
        cadena.agregarSimbolo(tokenActual.toStdString());
    }
}

QString VistaPruebaCadenaWidget::construirTextoTrazaDFA(
    const DFA& dfa, const ListaPasosDFA& pasos, bool procesable, bool aceptada,
    const std::string& estadoFinal) const {
    QString texto = "Estado inicial: " + QString::fromStdString(dfa.obtenerEstadoInicial());
    texto += "\n\n";
    const NodoPasoDFA* actual = pasos.obtenerPrimero();
    int numero = 1;
    while (actual != nullptr) {
        texto += QString("Paso %1\n").arg(numero);
        texto += QString::fromStdString(actual->origen) + " --" +
                 QString::fromStdString(actual->simbolo) + "--> " +
                 QString::fromStdString(actual->destino) + "\n\n";
        actual = actual->siguiente;
        ++numero;
    }
    if (!procesable) texto += "SIMULACIÓN DETENIDA\n\n";
    if (pasos.estaVacia()) texto += "No se procesaron símbolos (ε).\n\n";
    texto += "Estado alcanzado: " + QString::fromStdString(estadoFinal) + "\n";
    texto += "Resultado: " + (procesable ? estadoResultado(aceptada) : "CADENA NO PROCESABLE");
    return texto;
}

QString VistaPruebaCadenaWidget::construirTextoTrazaUnion(
    const DFAUnion& dfaUnion, const ListaPasosDFAUnion& pasos,
    bool procesable, bool aceptada,
    const std::string& estadoFinalDFA1, const std::string& estadoFinalDFA2) const {
    const QString estadoInicial = "(" + QString::fromStdString(dfaUnion.obtenerEstadoInicialDFA1()) + "," +
                                QString::fromStdString(dfaUnion.obtenerEstadoInicialDFA2()) + ")";
    const QString estadoFinal = "(" + QString::fromStdString(estadoFinalDFA1) + "," +
                                QString::fromStdString(estadoFinalDFA2) + ")";
    QString texto = "Estado inicial: " + estadoInicial + "\n\n";
    const NodoPasoDFAUnion* actual = pasos.obtenerPrimero();
    int numero = 1;
    while (actual != nullptr) {
        texto += QString("Paso %1\n(").arg(numero) +
                 QString::fromStdString(actual->origenDFA1) + "," +
                 QString::fromStdString(actual->origenDFA2) + ") --" +
                 QString::fromStdString(actual->simbolo) + "--> (" +
                 QString::fromStdString(actual->destinoDFA1) + "," +
                 QString::fromStdString(actual->destinoDFA2) + ")\n\n";
        actual = actual->siguiente;
        ++numero;
    }
    if (!procesable) texto += "SIMULACIÓN DETENIDA\n\n";
    if (pasos.estaVacia()) texto += "No se procesaron símbolos (ε).\n\n";
    texto += "Estado alcanzado: " + estadoFinal + "\n";
    texto += "Resultado: " + (procesable ? estadoResultado(aceptada) : "CADENA NO PROCESABLE");
    return texto;
}

void VistaPruebaCadenaWidget::evaluarCadena() {
    if (!editorDFA1->esDFAValido() || !editorDFA2->esDFAValido() || !vistaUnion->hayUnionGenerada()) {
        mensajeEstado->setText("Valida ambos DFA y genera la unión antes de evaluar.");
        return;
    }
    const DFAUnion* unionActual = vistaUnion->obtenerDFAUnion();
    if (unionActual == nullptr) {
        mensajeEstado->setText("No existe una unión válida para evaluar.");
        return;
    }
    CadenaEntrada cadena;
    construirCadenaEntrada(entradaCadena->text(), cadena);
    ResultadoTriple resultado;
    EvaluadorTriple evaluador;
    evaluador.evaluar(*dfa1, *dfa2, *unionActual, cadena, resultado);
    ListaPasosDFA pasosDFA1;
    ListaPasosDFA pasosDFA2;
    ListaPasosDFAUnion pasosUnion;
    bool aceptadaDFA1 = false;
    bool aceptadaDFA2 = false;
    bool aceptadaUnion = false;
    std::string estadoDFA1;
    std::string estadoDFA2;
    std::string estadoUnionDFA1;
    std::string estadoUnionDFA2;
    SimuladorDFA simuladorDFA;
    SimuladorDFAUnion simuladorUnion;
    const bool procesableDFA1 = simuladorDFA.simularConTraza(*dfa1, cadena, pasosDFA1, aceptadaDFA1, estadoDFA1);
    const bool procesableDFA2 = simuladorDFA.simularConTraza(*dfa2, cadena, pasosDFA2, aceptadaDFA2, estadoDFA2);
    const bool procesableUnion = simuladorUnion.simularConTraza(*unionActual, cadena, pasosUnion, aceptadaUnion, estadoUnionDFA1, estadoUnionDFA2);
    hayResultados = true;
    if (!resultado.procesable) {
        const QString invalido = QString::fromStdString(resultado.simboloInvalido);
        mensajeEstado->setText("CADENA NO PROCESABLE\nSímbolo inválido: " + invalido);
        actualizarEstiloResultado(resultadoDFA1, "CADENA NO PROCESABLE", true);
        actualizarEstiloResultado(resultadoDFA2, "CADENA NO PROCESABLE", true);
        actualizarEstiloResultado(resultadoUnion, "CADENA NO PROCESABLE", true);
    } else {
        mensajeEstado->setText("Evaluación completada.");
        actualizarEstiloResultado(resultadoDFA1, estadoResultado(resultado.aceptadaDFA1), !resultado.aceptadaDFA1);
        actualizarEstiloResultado(resultadoDFA2, estadoResultado(resultado.aceptadaDFA2), !resultado.aceptadaDFA2);
        actualizarEstiloResultado(resultadoUnion, estadoResultado(resultado.aceptadaUnion), !resultado.aceptadaUnion);
    }
    if (resultado.procesable) {
        estadoFinalDFA1Label->setText("Estado: " + QString::fromStdString(resultado.estadoFinalDFA1));
        estadoFinalDFA2Label->setText("Estado: " + QString::fromStdString(resultado.estadoFinalDFA2));
        estadoFinalUnionLabel->setText("Estado: (" + QString::fromStdString(resultado.estadoFinalUnionDFA1) + "," + QString::fromStdString(resultado.estadoFinalUnionDFA2) + ")");
    } else {
        estadoFinalDFA1Label->setText("Estado: —");
        estadoFinalDFA2Label->setText("Estado: —");
        estadoFinalUnionLabel->setText("Estado: —");
    }
    consistenciaLabel->setText(resultado.procesable
        ? (resultado.unionConsistente ? "DFA Unión = DFA1 OR DFA2\nCORRECTA" : "DFA Unión = DFA1 OR DFA2\nINCONSISTENCIA DETECTADA")
        : "DFA Unión = DFA1 OR DFA2\nNo comprobable: cadena no procesable");
    consistenciaLabel->setProperty("error", resultado.procesable && !resultado.unionConsistente);
    consistenciaLabel->style()->unpolish(consistenciaLabel);
    consistenciaLabel->style()->polish(consistenciaLabel);
    trazaDFA1->setPlainText(construirTextoTrazaDFA(*dfa1, pasosDFA1, procesableDFA1, aceptadaDFA1, estadoDFA1));
    trazaDFA2->setPlainText(construirTextoTrazaDFA(*dfa2, pasosDFA2, procesableDFA2, aceptadaDFA2, estadoDFA2));
    trazaUnion->setPlainText(construirTextoTrazaUnion(*unionActual, pasosUnion, procesableUnion, aceptadaUnion, estadoUnionDFA1, estadoUnionDFA2));
}

void VistaPruebaCadenaWidget::actualizarEstiloResultado(QLabel* etiquetaResultado,
                                                        const QString& estado,
                                                        bool error) {
    etiquetaResultado->setText(estado);
    etiquetaResultado->setProperty("error", error);
    etiquetaResultado->style()->unpolish(etiquetaResultado);
    etiquetaResultado->style()->polish(etiquetaResultado);
}

void VistaPruebaCadenaWidget::limpiarResultados() {
    hayResultados = false;
    resultadoDFA1->setText("Sin evaluar");
    resultadoDFA2->setText("Sin evaluar");
    resultadoUnion->setText("Sin evaluar");
    resultadoDFA1->setProperty("error", false);
    resultadoDFA2->setProperty("error", false);
    resultadoUnion->setProperty("error", false);
    estadoFinalDFA1Label->setText("Estado: —");
    estadoFinalDFA2Label->setText("Estado: —");
    estadoFinalUnionLabel->setText("Estado: —");
    consistenciaLabel->setText("DFA Unión = DFA1 OR DFA2\nPendiente de evaluación");
    consistenciaLabel->setProperty("error", false);
    trazaDFA1->clear();
    trazaDFA2->clear();
    trazaUnion->clear();
    resultadoDFA1->style()->unpolish(resultadoDFA1);
    resultadoDFA1->style()->polish(resultadoDFA1);
    resultadoDFA1->update();
    resultadoDFA2->style()->unpolish(resultadoDFA2);
    resultadoDFA2->style()->polish(resultadoDFA2);
    resultadoDFA2->update();
    resultadoUnion->style()->unpolish(resultadoUnion);
    resultadoUnion->style()->polish(resultadoUnion);
    resultadoUnion->update();
    consistenciaLabel->style()->unpolish(consistenciaLabel);
    consistenciaLabel->style()->polish(consistenciaLabel);
    consistenciaLabel->update();
}

void VistaPruebaCadenaWidget::invalidarResultados() {
    limpiarResultados();
    actualizarDisponibilidad();
    mensajeEstado->setText("Los autómatas cambiaron. Valida y genera nuevamente la unión.");
}

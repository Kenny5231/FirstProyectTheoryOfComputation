#include "gui/VistaPruebaCadenaWidget.h"

#include "automata/DFA.h"
#include "estructuras/CadenaEntrada.h"
#include "estructuras/ListaPasosDFA.h"
#include "estructuras/NodoPasoDFA.h"
#include "estructuras/ListaEstados.h"
#include "estructuras/ListaSimbolos.h"
#include "estructuras/NodoEstado.h"
#include "estructuras/NodoSimbolo.h"
#include "estructuras/NodoSimboloCadena.h"
#include "gui/EditorDFAWidget.h"
#include "gui/VisualizadorAutomataWidget.h"
#include "simulacion/SimuladorDFA.h"

#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QComboBox>
#include <QStyle>
#include <QVBoxLayout>

namespace {
QLabel* etiqueta(const QString& texto, const QString& nombre) {
    QLabel* resultado = new QLabel(texto);
    resultado->setObjectName(nombre);
    resultado->setWordWrap(true);
    return resultado;
}
}

VistaPruebaCadenaWidget::VistaPruebaCadenaWidget(
    DFA& dfa1Referencia, DFA& dfa2Referencia, EditorDFAWidget* editor1,
    EditorDFAWidget* editor2, QWidget* parent)
    : QWidget(parent),
      dfa1(&dfa1Referencia),
      dfa2(&dfa2Referencia),
      editorDFA1(editor1),
      editorDFA2(editor2),
      comboAutomata(nullptr),
      entradaCadena(nullptr),
      botonEvaluar(nullptr),
      etiquetaEstadoDFA(nullptr),
      mensajeEstado(nullptr),
      resultadoLabel(nullptr),
      detallesResultadoLabel(nullptr),
      procedimientoFormal(nullptr),
      recorridoSimple(nullptr),
      hayResultados(false),
      visualizador(nullptr),
      pasosDFAVisual(),
      pasoActual(0),
      indicadorPaso(nullptr),
      botonAnterior(nullptr),
      botonSiguiente(nullptr),
      botonReiniciar(nullptr) {
    crearInterfaz();
    conectarEventos();
    actualizarDisponibilidad();
}

void VistaPruebaCadenaWidget::crearInterfaz() {
    QVBoxLayout* principal = new QVBoxLayout(this);
    principal->setContentsMargins(32, 28, 32, 30);
    principal->setSpacing(12);
    principal->addWidget(etiqueta("SIMULACIÓN", "eyebrow"));
    principal->addWidget(etiqueta("Prueba de cadena", "pageTitle"));
    principal->addWidget(etiqueta("Evalúa una cadena sobre un autómata determinista.", "pageSub"));

    // Selector de autómata
    QFrame* selectorFrame = new QFrame;
    selectorFrame->setObjectName("testRequirements");
    QGridLayout* selectorLayout = new QGridLayout(selectorFrame);
    selectorLayout->setContentsMargins(18, 14, 18, 14);
    selectorLayout->setHorizontalSpacing(20);
    selectorLayout->setVerticalSpacing(6);
    selectorLayout->addWidget(etiqueta("AUTÓMATA A PROBAR", "testSectionTitle"), 0, 0);
    comboAutomata = new QComboBox;
    comboAutomata->addItem("DFA 1");
    comboAutomata->addItem("DFA 2");
    selectorLayout->addWidget(comboAutomata, 0, 1, Qt::AlignLeft);
    selectorLayout->addWidget(etiqueta("Estado:", "testRequirementName"), 1, 0);
    etiquetaEstadoDFA = etiqueta("Pendiente", "testRequirementStatus");
    selectorLayout->addWidget(etiquetaEstadoDFA, 1, 1);
    selectorLayout->setColumnStretch(2, 1);
    selectorLayout->setRowStretch(2, 1);
    principal->addWidget(selectorFrame);

    // Entrada de cadena
    principal->addWidget(etiqueta("CADENA", "testSectionTitle"));
    entradaCadena = new QLineEdit;
    entradaCadena->setPlaceholderText("Ejemplo: 1101");
    entradaCadena->setMinimumHeight(40);
    principal->addWidget(entradaCadena);
    principal->addWidget(etiqueta("Si el alfabeto usa símbolos individuales, puedes escribirlos juntos. "
                                   "También puedes separar símbolos mediante espacios.", "testHint"));
    
    botonEvaluar = new QPushButton("Evaluar cadena");
    botonEvaluar->setObjectName("validationButton");
    botonEvaluar->setCursor(Qt::PointingHandCursor);
    principal->addWidget(botonEvaluar, 0, Qt::AlignRight);
    
    mensajeEstado = etiqueta("Selecciona un autómata y valídalo para habilitar la evaluación.", "testMessage");
    principal->addWidget(mensajeEstado);

    // Resultado
    principal->addWidget(etiqueta("RESULTADO", "testSectionTitle"));
    resultadoLabel = etiqueta("Sin evaluar", "testResultStatus");
    resultadoLabel->setStyleSheet("font-size: 16px; font-weight: bold;");
    principal->addWidget(resultadoLabel);
    
    detallesResultadoLabel = etiqueta("", "testResultDetails");
    principal->addWidget(detallesResultadoLabel);

    // Procedimiento formal
    principal->addWidget(etiqueta("PROCEDIMIENTO FORMAL — δ̂", "testSectionTitle"));
    procedimientoFormal = new QPlainTextEdit;
    procedimientoFormal->setReadOnly(true);
    procedimientoFormal->setObjectName("testTrace");
    procedimientoFormal->setPlaceholderText("El procedimiento formal aparecerá aquí.");
    procedimientoFormal->setMinimumHeight(200);
    principal->addWidget(procedimientoFormal);

    // Recorrido simple
    principal->addWidget(etiqueta("RECORRIDO", "testSectionTitle"));
    recorridoSimple = new QPlainTextEdit;
    recorridoSimple->setReadOnly(true);
    recorridoSimple->setObjectName("testTrace");
    recorridoSimple->setPlaceholderText("El recorrido de estados aparecerá aquí.");
    recorridoSimple->setMinimumHeight(100);
    principal->addWidget(recorridoSimple);

    // Visualización gráfica
    principal->addWidget(etiqueta("VISUALIZACIÓN DEL RECORRIDO", "testSectionTitle"));
    visualizador = new VisualizadorAutomataWidget;
    principal->addWidget(visualizador);
    
    QHBoxLayout* controlesRecorrido = new QHBoxLayout;
    botonAnterior = new QPushButton("Anterior");
    indicadorPaso = etiqueta("Paso 0 / 0", "testStepIndicator");
    botonSiguiente = new QPushButton("Siguiente");
    botonReiniciar = new QPushButton("Reiniciar recorrido");
    controlesRecorrido->addWidget(botonAnterior);
    controlesRecorrido->addStretch();
    controlesRecorrido->addWidget(indicadorPaso);
    controlesRecorrido->addStretch();
    controlesRecorrido->addWidget(botonSiguiente);
    controlesRecorrido->addWidget(botonReiniciar);
    principal->addLayout(controlesRecorrido);
    
    principal->addStretch(1);
}

void VistaPruebaCadenaWidget::conectarEventos() {
    connect(botonEvaluar, &QPushButton::clicked, this, [this]() { evaluarCadena(); });
    connect(comboAutomata, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            [this]() { cambiarDFASeleccionado(); });
    
    connect(editorDFA1, &EditorDFAWidget::dfaModificado, this, [this]() {
        if (comboAutomata->currentIndex() == 0) {
            invalidarResultados();
        }
    });
    connect(editorDFA2, &EditorDFAWidget::dfaModificado, this, [this]() {
        if (comboAutomata->currentIndex() == 1) {
            invalidarResultados();
        }
    });
    connect(editorDFA1, &EditorDFAWidget::estadoValidacionCambiado,
            this, [this](bool) {
                if (comboAutomata->currentIndex() == 0) {
                    actualizarDisponibilidad();
                }
            });
    connect(editorDFA2, &EditorDFAWidget::estadoValidacionCambiado,
            this, [this](bool) {
                if (comboAutomata->currentIndex() == 1) {
                    actualizarDisponibilidad();
                }
            });
    
    connect(botonAnterior, &QPushButton::clicked, this, [this]() {
        if (pasoActual > 0) { 
            --pasoActual; 
            actualizarRecorrido(); 
        }
    });
    connect(botonSiguiente, &QPushButton::clicked, this, [this]() {
        if (pasoActual < pasosDFAVisual.cantidad()) { 
            ++pasoActual; 
            actualizarRecorrido(); 
        }
    });
    connect(botonReiniciar, &QPushButton::clicked, this, [this]() {
        pasoActual = 0;
        actualizarRecorrido();
    });
}

DFA* VistaPruebaCadenaWidget::obtenerDFASeleccionado() const {
    return comboAutomata->currentIndex() == 0 ? dfa1 : dfa2;
}

EditorDFAWidget* VistaPruebaCadenaWidget::obtenerEditorSeleccionado() const {
    return comboAutomata->currentIndex() == 0 ? editorDFA1 : editorDFA2;
}

const NodoPasoDFA* VistaPruebaCadenaWidget::obtenerPasoDFA(int indice) const {
    const NodoPasoDFA* actual = pasosDFAVisual.obtenerPrimero();
    int contador = 1;
    while (actual != nullptr) {
        if (contador == indice) return actual;
        actual = actual->siguiente;
        ++contador;
    }
    return nullptr;
}

bool VistaPruebaCadenaWidget::todosSimbolosSonDeUnCaracter(const DFA& dfa) const {
    const ListaSimbolos& alfabeto = dfa.obtenerAlfabeto();
    const NodoSimbolo* actual = alfabeto.obtenerPrimero();
    while (actual != nullptr) {
        if (actual->simbolo.length() != 1) {
            return false;
        }
        actual = actual->siguiente;
    }
    return true;
}

void VistaPruebaCadenaWidget::construirCadenaEntrada(const QString& texto, const DFA& dfa, 
                                                      CadenaEntrada& cadena) const {
    cadena.limpiar();
    
    if (texto.isEmpty()) {
        // Cadena vacía
        return;
    }
    
    bool todosUnCaracter = todosSimbolosSonDeUnCaracter(dfa);
    
    if (todosUnCaracter) {
        // Verificar si hay espacios
        bool tieneEspacios = false;
        for (int i = 0; i < texto.size(); ++i) {
            if (texto.at(i).isSpace()) {
                tieneEspacios = true;
                break;
            }
        }
        
        if (tieneEspacios) {
            // Procesar como símbolos separados por espacio
            QString tokenActual;
            for (int i = 0; i < texto.size(); ++i) {
                const QChar caracter = texto.at(i);
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
        } else {
            // Procesar carácter por carácter
            for (int i = 0; i < texto.size(); ++i) {
                cadena.agregarSimbolo(std::string(1, texto.at(i).toLatin1()));
            }
        }
    } else {
        // Símbolos multicarácter: requerir espacios
        QString tokenActual;
        for (int i = 0; i < texto.size(); ++i) {
            const QChar caracter = texto.at(i);
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
}

QString VistaPruebaCadenaWidget::construirProcedimientoFormal(
    const DFA& dfa, const CadenaEntrada& cadena, const ListaPasosDFA& pasos,
    bool procesable, bool aceptada, const std::string& estadoFinal) const {
    
    const std::string estadoInicial = dfa.obtenerEstadoInicial();
    QString resultado;
    
    // Primer paso: δ̂(q_inicial, ε) = q_inicial
    resultado += "δ̂(" + QString::fromStdString(estadoInicial) + ", ε) = " +
                 QString::fromStdString(estadoInicial) + "\n\n";
    
    if (pasos.estaVacia()) {
        // Solo cadena vacía
        // Determinar si es aceptada
        const ListaEstados& estadosFinales = dfa.obtenerEstadosFinales();
        const NodoEstado* nodo = estadosFinales.obtenerPrimero();
        bool esEstadoFinal = false;
        while (nodo != nullptr) {
            if (nodo->nombre == estadoInicial) {
                esEstadoFinal = true;
                break;
            }
            nodo = nodo->siguiente;
        }
        
        if (esEstadoFinal) {
            resultado += QString::fromStdString(estadoInicial) + " ∈ F\n\n";
            resultado += "∴ M reconoce ε\n";
            resultado += "∴ ε ∈ L(M)\n";
        } else {
            resultado += QString::fromStdString(estadoInicial) + " ∉ F\n\n";
            resultado += "∴ M no reconoce ε\n";
            resultado += "∴ ε ∉ L(M)\n";
        }
        return resultado;
    }
    
    // Procesar paso a paso
    QString prefijoAnterior;
    QString prefijoActual;
    
    const NodoPasoDFA* paso = pasos.obtenerPrimero();
    
    while (paso != nullptr) {
        prefijoAnterior = prefijoActual;
        prefijoActual += QString::fromStdString(paso->simbolo);
        
        const QString prefijoAnteriorMostrar =
            prefijoAnterior.isEmpty() ? QString::fromUtf8("ε") : prefijoAnterior;
        
        resultado += "δ̂(" + QString::fromStdString(estadoInicial) + ", " + prefijoActual + ")\n";
        resultado += "= δ(δ̂(" + QString::fromStdString(estadoInicial) + ", " +
                     prefijoAnteriorMostrar;
        resultado += "), " + QString::fromStdString(paso->simbolo) + ")\n";
        resultado += "= δ(" + QString::fromStdString(paso->origen) + ", " +
                     QString::fromStdString(paso->simbolo) + ")\n";
        resultado += "= " + QString::fromStdString(paso->destino) + "\n\n";
        
        paso = paso->siguiente;
    }
    
    // Conclusión
    resultado += "δ̂(" + QString::fromStdString(estadoInicial) + ", " + prefijoActual + ") = " +
                 QString::fromStdString(estadoFinal) + "\n\n";
    
    // Determinar si es estado final
    const ListaEstados& estadosFinales = dfa.obtenerEstadosFinales();
    const NodoEstado* nodo = estadosFinales.obtenerPrimero();
    bool esEstadoFinal = false;
    while (nodo != nullptr) {
        if (nodo->nombre == estadoFinal) {
            esEstadoFinal = true;
            break;
        }
        nodo = nodo->siguiente;
    }
    
    // Mostrar conjunto F
    resultado += "F = {";
    nodo = estadosFinales.obtenerPrimero();
    bool primero = true;
    while (nodo != nullptr) {
        if (!primero) resultado += ", ";
        resultado += QString::fromStdString(nodo->nombre);
        primero = false;
        nodo = nodo->siguiente;
    }
    resultado += "}\n\n";
    
    if (procesable) {
        if (esEstadoFinal) {
            resultado += QString::fromStdString(estadoFinal) + " ∈ F\n\n";
            resultado += "∴ M reconoce " + prefijoActual + "\n";
            resultado += "∴ " + prefijoActual + " ∈ L(M)\n";
        } else {
            resultado += QString::fromStdString(estadoFinal) + " ∉ F\n\n";
            resultado += "∴ M no reconoce " + prefijoActual + "\n";
            resultado += "∴ " + prefijoActual + " ∉ L(M)\n";
        }
    } else {
        std::string simboloInvalido;
        const NodoSimboloCadena* nodoCadena = cadena.obtenerPrimero();
        while (nodoCadena != nullptr) {
            if (!dfa.obtenerAlfabeto().existe(nodoCadena->simbolo)) {
                simboloInvalido = nodoCadena->simbolo;
                break;
            }
            nodoCadena = nodoCadena->siguiente;
        }
        resultado += "PROCESAMIENTO DETENIDO\n\n";
        if (!simboloInvalido.empty()) {
            resultado += QString::fromStdString(simboloInvalido) + " ∉ Σ\n\n";
        }
        resultado += "La cadena no puede ser procesada por M.\n";
        resultado += "RESULTADO: NO PROCESABLE\n";
    }
    
    return resultado;
}

QString VistaPruebaCadenaWidget::construirRecorridoSimple(const ListaPasosDFA& pasos) const {
    if (pasos.estaVacia()) {
        return "No se procesaron símbolos (ε).";
    }
    
    QString resultado;
    const NodoPasoDFA* paso = pasos.obtenerPrimero();
    
    while (paso != nullptr) {
        resultado += QString::fromStdString(paso->origen) + " --" +
                     QString::fromStdString(paso->simbolo) + "--> " +
                     QString::fromStdString(paso->destino) + "\n";
        paso = paso->siguiente;
    }
    
    return resultado;
}

void VistaPruebaCadenaWidget::evaluarCadena() {
    DFA* dfaSeleccionado = obtenerDFASeleccionado();
    EditorDFAWidget* editorSeleccionado = obtenerEditorSeleccionado();
    
    if (!editorSeleccionado->esDFAValido()) {
        const QString nombreDFA = comboAutomata->currentIndex() == 0 ? "DFA 1" : "DFA 2";
        mensajeEstado->setText("Valida " + nombreDFA + " antes de evaluar cadenas.");
        return;
    }
    
    CadenaEntrada cadena;
    construirCadenaEntrada(entradaCadena->text(), *dfaSeleccionado, cadena);
    
    ListaPasosDFA pasos;
    bool aceptada = false;
    std::string estadoFinal;
    
    SimuladorDFA simulador;
    const bool procesable = simulador.simularConTraza(*dfaSeleccionado, cadena, pasos, 
                                                       aceptada, estadoFinal);
    
    pasosDFAVisual.limpiar();
    const NodoPasoDFA* pasoFuente = pasos.obtenerPrimero();
    while (pasoFuente != nullptr) {
        pasosDFAVisual.agregarPaso(pasoFuente->origen, pasoFuente->simbolo, pasoFuente->destino);
        pasoFuente = pasoFuente->siguiente;
    }
    
    pasoActual = 0;
    hayResultados = true;
    
    if (!procesable) {
        resultadoLabel->setText("NO PROCESABLE");
        resultadoLabel->setProperty("error", true);
        detallesResultadoLabel->setText("La cadena contiene un símbolo que no pertenece al alfabeto.");
        mensajeEstado->setText("CADENA NO PROCESABLE");
    } else {
        const QString veredicto = aceptada ? "ACEPTADA" : "RECHAZADA";
        resultadoLabel->setText(veredicto);
        resultadoLabel->setProperty("error", !aceptada);
        
        const QString nombreDFA = comboAutomata->currentIndex() == 0 ? "DFA 1" : "DFA 2";
        QString detalles = "Autómata: " + nombreDFA + "\n";
        detalles += "Cadena: " + entradaCadena->text() + "\n";
        detalles += "Estado inicial: " + QString::fromStdString(dfaSeleccionado->obtenerEstadoInicial()) + "\n";
        detalles += "Estado alcanzado: " + QString::fromStdString(estadoFinal) + "\n";
        detallesResultadoLabel->setText(detalles);
        
        mensajeEstado->setText("Evaluación completada.");
    }
    
    resultadoLabel->style()->unpolish(resultadoLabel);
    resultadoLabel->style()->polish(resultadoLabel);
    
    procedimientoFormal->setPlainText(construirProcedimientoFormal(*dfaSeleccionado, cadena, 
                                                                   pasos, procesable, 
                                                                   aceptada, estadoFinal));
    recorridoSimple->setPlainText(construirRecorridoSimple(pasos));
    
    actualizarRecorrido();
}

void VistaPruebaCadenaWidget::actualizarEstiloResultado(QLabel* etiqueta, const QString& estado, bool error) {
    etiqueta->setText(estado);
    etiqueta->setProperty("error", error);
    etiqueta->style()->unpolish(etiqueta);
    etiqueta->style()->polish(etiqueta);
}

void VistaPruebaCadenaWidget::limpiarResultados() {
    hayResultados = false;
    resultadoLabel->setText("Sin evaluar");
    resultadoLabel->setProperty("error", false);
    detallesResultadoLabel->setText("");
    procedimientoFormal->clear();
    recorridoSimple->clear();
    pasosDFAVisual.limpiar();
    pasoActual = 0;
    resultadoLabel->style()->unpolish(resultadoLabel);
    resultadoLabel->style()->polish(resultadoLabel);
    resultadoLabel->update();
    visualizador->limpiar();
    indicadorPaso->setText("Paso 0 / 0");
}

void VistaPruebaCadenaWidget::actualizarRecorrido() {
    DFA* dfaSeleccionado = obtenerDFASeleccionado();
    if (dfaSeleccionado == nullptr || !hayResultados) {
        visualizador->limpiar();
        indicadorPaso->setText("Paso 0 / 0");
        return;
    }
    
    visualizador->mostrarDFA(*dfaSeleccionado);
    if (pasoActual == 0) {
        visualizador->resaltarEstadoDFA(dfaSeleccionado->obtenerEstadoInicial());
    } else {
        const NodoPasoDFA* paso = obtenerPasoDFA(pasoActual);
        if (paso != nullptr) {
            visualizador->resaltarTransicionDFA(paso->origen, paso->simbolo, paso->destino);
        }
    }
    indicadorPaso->setText(QString("Paso %1 / %2").arg(pasoActual).arg(pasosDFAVisual.cantidad()));
    botonAnterior->setEnabled(pasoActual > 0);
    botonSiguiente->setEnabled(pasoActual < pasosDFAVisual.cantidad());
}

void VistaPruebaCadenaWidget::actualizarDisponibilidad() {
    EditorDFAWidget* editorSeleccionado = obtenerEditorSeleccionado();
    const bool valido = editorSeleccionado->esDFAValido();
    
    etiquetaEstadoDFA->setText(valido ? "Válido" : 
                               (editorSeleccionado->estaValidado() ? "Inválido" : "Pendiente"));
    etiquetaEstadoDFA->setProperty("valid", valido);
    etiquetaEstadoDFA->style()->unpolish(etiquetaEstadoDFA);
    etiquetaEstadoDFA->style()->polish(etiquetaEstadoDFA);
    
    botonEvaluar->setEnabled(valido);
    const QString nombreDFA = comboAutomata->currentIndex() == 0 ? "DFA 1" : "DFA 2";
    botonEvaluar->setToolTip(valido ? "Evalúa la cadena." : "Valida " + nombreDFA + " antes de evaluar.");
    
    if (valido) {
        mensajeEstado->setText("Todo listo. Ingresa una cadena para evaluarla.");
    } else if (!editorSeleccionado->estaValidado()) {
        mensajeEstado->setText("Valida " + nombreDFA + " antes de evaluar cadenas.");
    } else {
        mensajeEstado->setText("Corrige y vuelve a validar " + nombreDFA + " antes de evaluar cadenas.");
    }
}

void VistaPruebaCadenaWidget::cambiarDFASeleccionado() {
    limpiarResultados();
    actualizarDisponibilidad();
}

void VistaPruebaCadenaWidget::invalidarResultados() {
    limpiarResultados();
    actualizarDisponibilidad();
}

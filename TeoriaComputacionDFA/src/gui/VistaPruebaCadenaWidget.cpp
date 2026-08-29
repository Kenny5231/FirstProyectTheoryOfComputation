#include "gui/VistaPruebaCadenaWidget.h"

#include "automata/DFA.h"
#include "automata/DFAUnion.h"
#include "estructuras/CadenaEntrada.h"
#include "estructuras/ListaPasosDFA.h"
#include "estructuras/ListaPasosDFAUnion.h"
#include "estructuras/NodoPasoDFA.h"
#include "estructuras/NodoPasoDFAUnion.h"
#include "estructuras/ListaEstados.h"
#include "estructuras/ListaEstadosCompuestos.h"
#include "estructuras/ListaSimbolos.h"
#include "estructuras/NodoEstado.h"
#include "estructuras/NodoEstadoCompuesto.h"
#include "estructuras/NodoSimbolo.h"
#include "estructuras/NodoSimboloCadena.h"
#include "gui/EditorDFAWidget.h"
#include "gui/VisualizadorAutomataWidget.h"
#include "gui/VistaUnionDFAWidget.h"
#include "simulacion/SimuladorDFA.h"
#include "simulacion/SimuladorDFAUnion.h"

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

QString estadoCompuesto(const std::string& primero, const std::string& segundo) {
    return "(" + QString::fromStdString(primero) + "," + QString::fromStdString(segundo) + ")";
}
}

VistaPruebaCadenaWidget::VistaPruebaCadenaWidget(
    DFA& dfa1Referencia, DFA& dfa2Referencia, EditorDFAWidget* editor1,
    EditorDFAWidget* editor2, VistaUnionDFAWidget* vistaUnionRef, QWidget* parent)
    : QWidget(parent),
      dfa1(&dfa1Referencia),
      dfa2(&dfa2Referencia),
      editorDFA1(editor1),
      editorDFA2(editor2),
      vistaUnion(vistaUnionRef),
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
      pasosUnionVisual(),
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
    comboAutomata->addItem("DFA Unión");
    selectorLayout->addWidget(comboAutomata, 0, 1, Qt::AlignLeft);
    selectorLayout->addWidget(etiqueta("Estado:", "testRequirementName"), 1, 0);
    etiquetaEstadoDFA = etiqueta("Pendiente", "testRequirementStatus");
    selectorLayout->addWidget(etiquetaEstadoDFA, 1, 1);
    selectorLayout->setColumnStretch(2, 1);
    selectorLayout->setRowStretch(2, 1);
    principal->addWidget(selectorFrame);

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

    principal->addWidget(etiqueta("RESULTADO", "testSectionTitle"));
    resultadoLabel = etiqueta("Sin evaluar", "testResultStatus");
    resultadoLabel->setStyleSheet("font-size: 16px; font-weight: bold;");
    principal->addWidget(resultadoLabel);

    detallesResultadoLabel = etiqueta("", "testResultDetails");
    principal->addWidget(detallesResultadoLabel);

    principal->addWidget(etiqueta("PROCEDIMIENTO FORMAL — δ̂", "testSectionTitle"));
    procedimientoFormal = new QPlainTextEdit;
    procedimientoFormal->setReadOnly(true);
    procedimientoFormal->setObjectName("testTrace");
    procedimientoFormal->setPlaceholderText("El procedimiento formal aparecerá aquí.");
    procedimientoFormal->setMinimumHeight(200);
    principal->addWidget(procedimientoFormal);

    principal->addWidget(etiqueta("RECORRIDO", "testSectionTitle"));
    recorridoSimple = new QPlainTextEdit;
    recorridoSimple->setReadOnly(true);
    recorridoSimple->setObjectName("testTrace");
    recorridoSimple->setPlaceholderText("El recorrido de estados aparecerá aquí.");
    recorridoSimple->setMinimumHeight(100);
    principal->addWidget(recorridoSimple);

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
        if (comboAutomata->currentIndex() == DFA1_SELECCIONADO ||
            comboAutomata->currentIndex() == UNION_SELECCIONADO) {
            invalidarResultados();
        }
    });
    connect(editorDFA2, &EditorDFAWidget::dfaModificado, this, [this]() {
        if (comboAutomata->currentIndex() == DFA2_SELECCIONADO ||
            comboAutomata->currentIndex() == UNION_SELECCIONADO) {
            invalidarResultados();
        }
    });
    connect(editorDFA1, &EditorDFAWidget::estadoValidacionCambiado,
            this, [this](bool) {
                actualizarDisponibilidad();
            });
    connect(editorDFA2, &EditorDFAWidget::estadoValidacionCambiado,
            this, [this](bool) {
                actualizarDisponibilidad();
            });

    if (vistaUnion != nullptr) {
        connect(vistaUnion, &VistaUnionDFAWidget::estadoUnionCambiado,
                this, [this](bool) {
                    if (comboAutomata->currentIndex() == UNION_SELECCIONADO) {
                        invalidarResultados();
                    }
                    actualizarDisponibilidad();
                });
    }

    connect(botonAnterior, &QPushButton::clicked, this, [this]() {
        if (pasoActual > 0) {
            --pasoActual;
            actualizarRecorrido();
        }
    });
    connect(botonSiguiente, &QPushButton::clicked, this, [this]() {
        const int total = (obtenerAutomataSeleccionado() == UNION_SELECCIONADO)
                              ? pasosUnionVisual.cantidad()
                              : pasosDFAVisual.cantidad();
        if (pasoActual < total) {
            ++pasoActual;
            actualizarRecorrido();
        }
    });
    connect(botonReiniciar, &QPushButton::clicked, this, [this]() {
        pasoActual = 0;
        actualizarRecorrido();
    });
}

VistaPruebaCadenaWidget::AutomataSeleccionado VistaPruebaCadenaWidget::obtenerAutomataSeleccionado() const {
    const int indice = comboAutomata->currentIndex();
    if (indice == 1) return DFA2_SELECCIONADO;
    if (indice == 2) return UNION_SELECCIONADO;
    return DFA1_SELECCIONADO;
}

DFA* VistaPruebaCadenaWidget::obtenerDFASeleccionado() const {
    return obtenerAutomataSeleccionado() == DFA2_SELECCIONADO ? dfa2 : dfa1;
}

EditorDFAWidget* VistaPruebaCadenaWidget::obtenerEditorSeleccionado() const {
    return obtenerAutomataSeleccionado() == DFA2_SELECCIONADO ? editorDFA2 : editorDFA1;
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

const NodoPasoDFAUnion* VistaPruebaCadenaWidget::obtenerPasoUnion(int indice) const {
    const NodoPasoDFAUnion* actual = pasosUnionVisual.obtenerPrimero();
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
        return;
    }

    bool todosUnCaracter = todosSimbolosSonDeUnCaracter(dfa);

    if (todosUnCaracter) {
        bool tieneEspacios = false;
        for (int i = 0; i < texto.size(); ++i) {
            if (texto.at(i).isSpace()) {
                tieneEspacios = true;
                break;
            }
        }

        if (tieneEspacios) {
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
            for (int i = 0; i < texto.size(); ++i) {
                cadena.agregarSimbolo(std::string(1, texto.at(i).toLatin1()));
            }
        }
    } else {
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

void VistaPruebaCadenaWidget::construirCadenaEntradaUnion(const QString& texto,
                                                          const DFAUnion& dfaUnion,
                                                          CadenaEntrada& cadena) const {
    cadena.limpiar();
    if (texto.isEmpty()) {
        return;
    }

    const DFA* dfaFuente = dfa1;
    if (dfaFuente->obtenerAlfabeto().cantidad() == 0 &&
        dfa2->obtenerAlfabeto().cantidad() > 0) {
        dfaFuente = dfa2;
    }

    bool todosUnCaracter = true;
    const ListaSimbolos& alfabeto = dfaFuente->obtenerAlfabeto();
    const NodoSimbolo* nodoS = alfabeto.obtenerPrimero();
    while (nodoS != nullptr) {
        if (nodoS->simbolo.length() != 1) {
            todosUnCaracter = false;
            break;
        }
        nodoS = nodoS->siguiente;
    }

    if (todosUnCaracter) {
        bool tieneEspacios = false;
        for (int i = 0; i < texto.size(); ++i) {
            if (texto.at(i).isSpace()) {
                tieneEspacios = true;
                break;
            }
        }
        if (tieneEspacios) {
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
            for (int i = 0; i < texto.size(); ++i) {
                cadena.agregarSimbolo(std::string(1, texto.at(i).toLatin1()));
            }
        }
    } else {
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

bool VistaPruebaCadenaWidget::todosSimbolosEnAlfabeto(const DFAUnion& dfaUnion,
                                                      const CadenaEntrada& cadena) const {
    const NodoSimboloCadena* nodoCadena = cadena.obtenerPrimero();
    while (nodoCadena != nullptr) {
        if (!dfaUnion.obtenerAlfabeto().existe(nodoCadena->simbolo)) {
            return false;
        }
        nodoCadena = nodoCadena->siguiente;
    }
    return true;
}

QString VistaPruebaCadenaWidget::simboloInvalidoUnion(const DFAUnion& dfaUnion,
                                                      const CadenaEntrada& cadena) const {
    const NodoSimboloCadena* nodoCadena = cadena.obtenerPrimero();
    while (nodoCadena != nullptr) {
        if (!dfaUnion.obtenerAlfabeto().existe(nodoCadena->simbolo)) {
            return QString::fromStdString(nodoCadena->simbolo);
        }
        nodoCadena = nodoCadena->siguiente;
    }
    return QString();
}

bool VistaPruebaCadenaWidget::estaEnEstadosFinalesUnion(const DFAUnion& dfaUnion,
                                                        const std::string& estadoDFA1,
                                                        const std::string& estadoDFA2) const {
    return dfaUnion.obtenerEstadosFinales().existe(estadoDFA1, estadoDFA2);
}

QString VistaPruebaCadenaWidget::construirProcedimientoFormal(
    const DFA& dfa, const CadenaEntrada& cadena, const ListaPasosDFA& pasos,
    bool procesable, bool aceptada, const std::string& estadoFinal) const {

    const std::string estadoInicial = dfa.obtenerEstadoInicial();
    QString resultado;

    resultado += "δ̂(" + QString::fromStdString(estadoInicial) + ", ε) = " +
                 QString::fromStdString(estadoInicial) + "\n\n";

    if (cadena.estaVacia()) {
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
            resultado += "RESULTADO: ACEPTADA\n";
        } else {
            resultado += QString::fromStdString(estadoInicial) + " ∉ F\n\n";
            resultado += "∴ M no reconoce ε\n";
            resultado += "∴ ε ∉ L(M)\n";
            resultado += "RESULTADO: RECHAZADA\n";
        }
        return resultado;
    }

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

    if (!pasos.estaVacia()) {
        resultado += "δ̂(" + QString::fromStdString(estadoInicial) + ", " + prefijoActual + ") = " +
                     QString::fromStdString(estadoFinal) + "\n\n";
    }

    if (procesable) {
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

        if (esEstadoFinal) {
            resultado += QString::fromStdString(estadoFinal) + " ∈ F\n\n";
            resultado += "∴ M reconoce " + prefijoActual + "\n";
            resultado += "∴ " + prefijoActual + " ∈ L(M)\n";
            resultado += "RESULTADO: ACEPTADA\n";
        } else {
            resultado += QString::fromStdString(estadoFinal) + " ∉ F\n\n";
            resultado += "∴ M no reconoce " + prefijoActual + "\n";
            resultado += "∴ " + prefijoActual + " ∉ L(M)\n";
            resultado += "RESULTADO: RECHAZADA\n";
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

QString VistaPruebaCadenaWidget::construirProcedimientoFormalUnion(
    const DFAUnion& dfaUnion, const CadenaEntrada& cadena,
    const ListaPasosDFAUnion& pasos, bool procesable, bool aceptada,
    const std::string& estadoFinalDFA1, const std::string& estadoFinalDFA2) const {

    const std::string qIni1 = dfaUnion.obtenerEstadoInicialDFA1();
    const std::string qIni2 = dfaUnion.obtenerEstadoInicialDFA2();
    const QString estadoInicialCompuesto = estadoCompuesto(qIni1, qIni2);
    QString resultado;

    resultado += "δ̂U(" + estadoInicialCompuesto + ", ε) = " + estadoInicialCompuesto + "\n\n";

    if (cadena.estaVacia()) {
        const bool esFinal = estaEnEstadosFinalesUnion(dfaUnion, qIni1, qIni2);
        if (esFinal) {
            resultado += estadoInicialCompuesto + " ∈ FU\n\n";
            resultado += "∴ MU reconoce ε\n";
            resultado += "∴ ε ∈ L(MU)\n";
            resultado += "RESULTADO: ACEPTADA\n";
        } else {
            resultado += estadoInicialCompuesto + " ∉ FU\n\n";
            resultado += "∴ MU no reconoce ε\n";
            resultado += "∴ ε ∉ L(MU)\n";
            resultado += "RESULTADO: RECHAZADA\n";
        }
        return resultado;
    }

    QString prefijoAnterior;
    QString prefijoActual;

    const NodoPasoDFAUnion* paso = pasos.obtenerPrimero();
    while (paso != nullptr) {
        prefijoAnterior = prefijoActual;
        prefijoActual += QString::fromStdString(paso->simbolo);

        const QString prefijoAnteriorMostrar =
            prefijoAnterior.isEmpty() ? QString::fromUtf8("ε") : prefijoAnterior;

        const QString origen = estadoCompuesto(paso->origenDFA1, paso->origenDFA2);
        const QString destino = estadoCompuesto(paso->destinoDFA1, paso->destinoDFA2);

        resultado += "δ̂U(" + estadoInicialCompuesto + ", " + prefijoActual + ")\n";
        resultado += "= δU(δ̂U(" + estadoInicialCompuesto + ", " + prefijoAnteriorMostrar + "), " +
                     QString::fromStdString(paso->simbolo) + ")\n";
        resultado += "= δU(" + origen + ", " + QString::fromStdString(paso->simbolo) + ")\n";
        resultado += "= (δ1(" + QString::fromStdString(paso->origenDFA1) + "," +
                     QString::fromStdString(paso->simbolo) + "), δ2(" +
                     QString::fromStdString(paso->origenDFA2) + "," +
                     QString::fromStdString(paso->simbolo) + "))\n";
        resultado += "= " + destino + "\n\n";

        paso = paso->siguiente;
    }

    if (procesable) {
        const QString estadoFinalCompuesto = estadoCompuesto(estadoFinalDFA1, estadoFinalDFA2);
        if (!pasos.estaVacia()) {
            resultado += "δ̂U(" + estadoInicialCompuesto + ", " + prefijoActual + ") = " +
                         estadoFinalCompuesto + "\n\n";
        }

        resultado += "FU = {";
        const NodoEstadoCompuesto* nodoFinal = dfaUnion.obtenerEstadosFinales().obtenerPrimero();
        bool primero = true;
        while (nodoFinal != nullptr) {
            if (!primero) resultado += ", ";
            resultado += estadoCompuesto(nodoFinal->estadoDFA1, nodoFinal->estadoDFA2);
            primero = false;
            nodoFinal = nodoFinal->siguiente;
        }
        resultado += "}\n\n";

        if (estaEnEstadosFinalesUnion(dfaUnion, estadoFinalDFA1, estadoFinalDFA2)) {
            resultado += estadoFinalCompuesto + " ∈ FU\n\n";
            resultado += "∴ MU reconoce " + prefijoActual + "\n";
            resultado += "∴ " + prefijoActual + " ∈ L(MU)\n";
            resultado += "RESULTADO: ACEPTADA\n";
        } else {
            resultado += estadoFinalCompuesto + " ∉ FU\n\n";
            resultado += "∴ MU no reconoce " + prefijoActual + "\n";
            resultado += "∴ " + prefijoActual + " ∉ L(MU)\n";
            resultado += "RESULTADO: RECHAZADA\n";
        }
    } else {
        const QString simboloInvalido = simboloInvalidoUnion(dfaUnion, cadena);
        resultado += "PROCESAMIENTO DETENIDO\n\n";
        if (!simboloInvalido.isEmpty()) {
            resultado += simboloInvalido + " ∉ ΣU\n\n";
        }
        resultado += "La cadena no puede ser procesada por MU.\n";
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

QString VistaPruebaCadenaWidget::construirRecorridoSimpleUnion(const ListaPasosDFAUnion& pasos) const {
    if (pasos.estaVacia()) {
        return "No se procesaron símbolos (ε).";
    }

    QString resultado;
    const NodoPasoDFAUnion* paso = pasos.obtenerPrimero();

    while (paso != nullptr) {
        resultado += estadoCompuesto(paso->origenDFA1, paso->origenDFA2) + " --" +
                     QString::fromStdString(paso->simbolo) + "--> " +
                     estadoCompuesto(paso->destinoDFA1, paso->destinoDFA2) + "\n";
        paso = paso->siguiente;
    }

    return resultado;
}

void VistaPruebaCadenaWidget::evaluarCadena() {
    if (obtenerAutomataSeleccionado() == UNION_SELECCIONADO) {
        evaluarUnion();
    } else {
        evaluarDFAIndividual();
    }
}

void VistaPruebaCadenaWidget::evaluarDFAIndividual() {
    DFA* dfaSeleccionado = obtenerDFASeleccionado();
    EditorDFAWidget* editorSeleccionado = obtenerEditorSeleccionado();

    if (!editorSeleccionado->esDFAValido()) {
        const QString nombreDFA =
            obtenerAutomataSeleccionado() == DFA2_SELECCIONADO ? "DFA 2" : "DFA 1";
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

    pasosUnionVisual.limpiar();
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

        const QString nombreDFA =
            obtenerAutomataSeleccionado() == DFA2_SELECCIONADO ? "DFA 2" : "DFA 1";
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

void VistaPruebaCadenaWidget::evaluarUnion() {
    if (vistaUnion == nullptr || !vistaUnion->hayUnionGenerada()) {
        mensajeEstado->setText("Genera primero el DFA Unión para poder probar cadenas sobre él.");
        return;
    }

    const DFAUnion* unionSeleccionada = vistaUnion->obtenerDFAUnion();
    if (unionSeleccionada == nullptr) {
        mensajeEstado->setText("Genera primero el DFA Unión para poder probar cadenas sobre él.");
        return;
    }

    CadenaEntrada cadena;
    construirCadenaEntradaUnion(entradaCadena->text(), *unionSeleccionada, cadena);

    ListaPasosDFAUnion pasos;
    bool aceptada = false;
    std::string estadoFinalDFA1;
    std::string estadoFinalDFA2;

    SimuladorDFAUnion simulador;
    bool procesable = simulador.simularConTraza(*unionSeleccionada, cadena, pasos,
                                                aceptada, estadoFinalDFA1,
                                                estadoFinalDFA2);

    if (procesable && !todosSimbolosEnAlfabeto(*unionSeleccionada, cadena)) {
        procesable = false;
    }

    pasosDFAVisual.limpiar();
    pasosUnionVisual.limpiar();
    const NodoPasoDFAUnion* pasoFuente = pasos.obtenerPrimero();
    while (pasoFuente != nullptr) {
        pasosUnionVisual.agregarPaso(pasoFuente->origenDFA1, pasoFuente->origenDFA2,
                                     pasoFuente->simbolo, pasoFuente->destinoDFA1,
                                     pasoFuente->destinoDFA2);
        pasoFuente = pasoFuente->siguiente;
    }

    pasoActual = 0;
    hayResultados = true;

    const QString estadoInicialUnion =
        estadoCompuesto(unionSeleccionada->obtenerEstadoInicialDFA1(),
                        unionSeleccionada->obtenerEstadoInicialDFA2());
    const QString estadoFinalUnion =
        estadoCompuesto(estadoFinalDFA1, estadoFinalDFA2);

    if (!procesable) {
        resultadoLabel->setText("NO PROCESABLE");
        resultadoLabel->setProperty("error", true);
        detallesResultadoLabel->setText("La cadena contiene un símbolo que no pertenece al alfabeto ΣU.");
        mensajeEstado->setText("CADENA NO PROCESABLE");
    } else {
        const QString veredicto = aceptada ? "ACEPTADA" : "RECHAZADA";
        resultadoLabel->setText(veredicto);
        resultadoLabel->setProperty("error", !aceptada);

        QString detalles = "Autómata: DFA Unión\n";
        detalles += "Cadena: " + entradaCadena->text() + "\n";
        detalles += "Estado inicial: " + estadoInicialUnion + "\n";
        detalles += "Estado alcanzado: " + estadoFinalUnion + "\n";
        detallesResultadoLabel->setText(detalles);

        mensajeEstado->setText("Evaluación completada.");
    }

    resultadoLabel->style()->unpolish(resultadoLabel);
    resultadoLabel->style()->polish(resultadoLabel);

    procedimientoFormal->setPlainText(construirProcedimientoFormalUnion(
        *unionSeleccionada, cadena, pasos, procesable, aceptada,
        estadoFinalDFA1, estadoFinalDFA2));
    recorridoSimple->setPlainText(construirRecorridoSimpleUnion(pasos));

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
    pasosUnionVisual.limpiar();
    pasoActual = 0;
    resultadoLabel->style()->unpolish(resultadoLabel);
    resultadoLabel->style()->polish(resultadoLabel);
    resultadoLabel->update();
    visualizador->limpiar();
    indicadorPaso->setText("Paso 0 / 0");
}

void VistaPruebaCadenaWidget::actualizarRecorrido() {
    const AutomataSeleccionado automata = obtenerAutomataSeleccionado();

    if (automata == UNION_SELECCIONADO) {
        if (vistaUnion == nullptr || !vistaUnion->hayUnionGenerada() || !hayResultados) {
            visualizador->limpiar();
            indicadorPaso->setText("Paso 0 / 0");
            return;
        }
        const DFAUnion* unionSeleccionada = vistaUnion->obtenerDFAUnion();
        if (unionSeleccionada == nullptr) {
            visualizador->limpiar();
            return;
        }
        visualizador->mostrarDFAUnion(*unionSeleccionada);
        if (pasoActual == 0) {
            visualizador->resaltarEstadoUnion(unionSeleccionada->obtenerEstadoInicialDFA1(),
                                              unionSeleccionada->obtenerEstadoInicialDFA2());
        } else {
            const NodoPasoDFAUnion* paso = obtenerPasoUnion(pasoActual);
            if (paso != nullptr) {
                visualizador->resaltarTransicionUnion(paso->origenDFA1, paso->origenDFA2,
                                                      paso->simbolo, paso->destinoDFA1,
                                                      paso->destinoDFA2);
            }
        }
        indicadorPaso->setText(QString("Paso %1 / %2").arg(pasoActual).arg(pasosUnionVisual.cantidad()));
        botonAnterior->setEnabled(pasoActual > 0);
        botonSiguiente->setEnabled(pasoActual < pasosUnionVisual.cantidad());
        return;
    }

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
    const AutomataSeleccionado automata = obtenerAutomataSeleccionado();

    if (automata == UNION_SELECCIONADO) {
        const bool valido1 = editorDFA1->esDFAValido();
        const bool valido2 = editorDFA2->esDFAValido();
        const bool unionGenerada = (vistaUnion != nullptr) && vistaUnion->hayUnionGenerada();

        bool disponible = false;
        QString mensaje;
        if (!valido1 || !valido2) {
            mensaje = "Valida DFA 1 y DFA 2 antes de evaluar sobre la Unión.";
        } else if (!unionGenerada) {
            mensaje = "Genera primero el DFA Unión para poder probar cadenas sobre él.";
        } else {
            disponible = true;
            mensaje = "Todo listo. Ingresa una cadena para evaluarla sobre el DFA Unión.";
        }

        etiquetaEstadoDFA->setText(disponible ? "Válido"
                                  : (valido1 && valido2 ? "NO DISPONIBLE" : "Inválido"));
        etiquetaEstadoDFA->setProperty("valid", disponible);
        etiquetaEstadoDFA->style()->unpolish(etiquetaEstadoDFA);
        etiquetaEstadoDFA->style()->polish(etiquetaEstadoDFA);

        botonEvaluar->setEnabled(disponible);
        botonEvaluar->setToolTip(disponible ? "Evalúa la cadena sobre el DFA Unión."
                                            : "No se puede evaluar la Unión aún.");
        mensajeEstado->setText(mensaje);
        return;
    }

    EditorDFAWidget* editorSeleccionado = obtenerEditorSeleccionado();
    const bool valido = editorSeleccionado->esDFAValido();

    etiquetaEstadoDFA->setText(valido ? "Válido" :
                               (editorSeleccionado->estaValidado() ? "Inválido" : "Pendiente"));
    etiquetaEstadoDFA->setProperty("valid", valido);
    etiquetaEstadoDFA->style()->unpolish(etiquetaEstadoDFA);
    etiquetaEstadoDFA->style()->polish(etiquetaEstadoDFA);

    botonEvaluar->setEnabled(valido);
    const QString nombreDFA =
        obtenerAutomataSeleccionado() == DFA2_SELECCIONADO ? "DFA 2" : "DFA 1";
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

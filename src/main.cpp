/**
 *   Nome: Pedro Leonel de Lorena, Leonardo Ferrarese Correa, Lais Rodrigues Sevilhano & Luigi Arnosti Reginato
 *  Descrição: Neste modulo de publish nos coletamos os hex do controle remoto e fizemos com que toda vez que apertasse o botao ele emitsse para a tela oq o hex faz.
 *  Projeto: Modulo de publicar os comandos feito na tela retratil
 *  Data: 21/05
 *  Versão: 0.0.7
 */

#include <Arduino.h>
#include "WiFiManager.h"
#include "MqttManager.h"
#include "DebugManager.h"
#include <ArduinoJson.h>
#include <secrets.h>
#include <Bounce2.h>
#include <TelaProjecaoRF.h>

//=========================
//* Variaveis globais

const unsigned long tempoInicio = 0;
unsigned long tempo = 0;
unsigned long tempoDecorrido = 0;
unsigned long tempoPause;
unsigned long tempoMaximo = 20000;
bool tempoParado = false;

bool UP = false;
bool DOWN = false;
bool PAUSE = false;

const char topicoComandoUp[] = "senai134/publisherTelaRetratil/esp32/comando";

const uint8_t ENDERECO_DA_MINHA_TELA[TelaProjecaoRF::TAMANHO_ENDERECO] = {
    0xCD, 0x4E, 0x0A, 0x01, 0x00};

const uint8_t PINO_TX = 7;
const uint8_t PINO_RX = 6;

//=========================

//=========================
//* CRIAÇÃO DE OBJETOS

Bounce BOTAOUP = Bounce();
Bounce BOTAODOWN = Bounce();
Bounce BOTAOPAUSE = Bounce();

TelaProjecaoRF telaRF(PINO_TX, PINO_RX);
//=========================

//=========================
//* INVOCAR AS FUNÇÕES

void postarComando();

//=========================

void setup()
{
  configurarDebug();
  conectarWiFi();
  configurarMQTT();
  conectarMQTT();
  registrarCallbackMensagem(tratarMensagemRecebida);

  Serial.begin(9600);
  delay(500);

  telaRF.begin(&Serial);
  telaRF.setInverterSinal(true); // Altere para false se a tela não responder
                                 // Temporário -->
  Serial.println("TelaProjecaoRF — Controle via Serial");
  Serial.println("Comandos: c=CIMA  p=PARAR  b=BAIXO  l=LEARN\n");
}

void loop()
{
  garantirWiFiConectado();
  garantirMQTTConectado();
  loopMQTT();

  telaRF.update();
  /////////////////////////////////
}

/*
  A ideia é que o botaoUP e down tenha a mesma logica pois ambos tem a mesma função, só que um sobe e o outro desce, e o botaoPAUSE tem a função de pausar o tempo, ou seja, quando ele for apertado ele salva o tempo decorrido e para de contar, e quando ele for apertado novamente ele continua contando de onde parou.
*/

void enviarRF()
{
  if (UP)
  {
    Serial.println(">> Subindo tela...");
    telaRF.enviarCima(ENDERECO_DA_MINHA_TELA);
    UP = false;
  }

  if (DOWN)
  {
    Serial.println(">> Baixando tela...");
    telaRF.enviarBaixo(ENDERECO_DA_MINHA_TELA);
    DOWN = false;
  }

  if (PAUSE)
  {
    Serial.println(">> Parando tela...");
    telaRF.enviarParar(ENDERECO_DA_MINHA_TELA);
    PAUSE = false;
  }
}

void tratarJsonComando(const String &mensagem)
{
  JsonDocument doc;

  DeserializationError erro = deserializeJson(doc, mensagem);

  if (erro)
  {
    debugErro("Erro ao desserializar o JSON: " + String(erro.c_str()));
    return;
  }

  if (doc["telaRetratil"].is<JsonObject>())
  {
    if (doc["telaRetratil"]["UP"].is<bool>())
      UP = doc["telaRetratil"]["UP"].as<bool>();

    if (doc["telaRetratil"]["DOWN"].is<bool>())
      DOWN = doc["telaRetratil"]["DOWN"].as<bool>();

    if (doc["telaRetratil"]["PAUSE"].is<bool>())
      PAUSE = doc["telaRetratil"]["PAUSE"].as<bool>();
  }
  Serial.println(UP);
  Serial.println(DOWN);
  Serial.println(PAUSE);

  enviarRF();
}

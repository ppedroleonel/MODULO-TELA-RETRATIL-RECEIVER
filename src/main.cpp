/**
 *   Nome: Pedro Leonel de Lorena, Leonardo Ferrarese Correa, Lais Rodrigues Sevilhano & Luigi Arnosti Reginato
 *  Descrição: Neste modulo de publish nos coletamos os hex do controle remoto e fizemos com que toda vez que apertasse o botao ele emitsse para a tela oq o hex faz.
 *  Projeto: Modulo de publicar os comandos feito na tela retratil
 *  Data: 01/06/2026
 *  Versão: 1.0.2
 *  */

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

const char TOPICO_COMANDO[] = "senai134/publisherTelaRetratil/esp32/comando";
const unsigned long tempoInicio = 0;
unsigned long tempo = 0;
unsigned long tempoDecorrido = 0;
unsigned long tempoPause;
unsigned long tempoMaximo = 20000;
bool tempoParado = false;

int8_t Tela = 0;

bool SendUP = false;
bool SendDOWN = false;
bool SendPAUSE = false;

const char topicoComandoUp[] = "senai134/publisherTelaRetratil/esp32/comando";

const uint8_t ENDERECO_TELA_0[TelaProjecaoRF::TAMANHO_ENDERECO] = {
    0xCD, 0x4E, 0x0A, 0x01, 0x00};
const uint8_t ENDERECO_TELA_1[TelaProjecaoRF::TAMANHO_ENDERECO] = {
    0xCD, 0x4B, 0xF6, 0x01, 0x00};

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
void tratarJsonComando(const String &mensagem);
void tratarMensagemRecebida(const char *topico, const String &mensagem);
//=========================

void setup()
{
  configurarDebug();
  conectarWiFi();
  configurarMQTT();
  conectarMQTT();
  registrarCallBackMensagem(tratarMensagemRecebida);

  Serial.begin(9600);
  // delay(500);

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
  if (Tela == 0)
  {
    if (SendUP)
    {
      Serial.println(">> Subindo tela... 0");
      telaRF.enviarCima(ENDERECO_TELA_0);
      SendUP = false;
    }

    if (SendDOWN)
    {
      Serial.println(">> Baixando tela... 0");
      telaRF.enviarBaixo(ENDERECO_TELA_0);
      SendDOWN = false;
    }

    if (SendPAUSE)
    {
      Serial.println(">> Parando tela... 0");
      telaRF.enviarParar(ENDERECO_TELA_0);
      SendPAUSE = false;
    }
  }
  else if (Tela == 1)
  {

    if (SendUP)
    {
      Serial.println(">> Subindo tela... 1");
      telaRF.enviarCima(ENDERECO_TELA_1);
      SendUP = false;
    }

    if (SendDOWN)
    {
      Serial.println(">> Baixando tela... 1");
      telaRF.enviarBaixo(ENDERECO_TELA_1);
      SendDOWN = false;
    }

    if (SendPAUSE)
    {
      Serial.println(">> Parando tela... 1");
      telaRF.enviarParar(ENDERECO_TELA_1);
      SendPAUSE = false;
    }
  }
}

  void tratarMensagemRecebida(const char *topico, const String &mensagem)
  {
    debugInfo("==================================");
    debugInfo("Mensagem recebida na aplicação");
    debugInfo("==================================");

    if (topico == nullptr)
    {
      debugErro("Tópico MQTT inválido");
      return;
    }

    debugInfo("Tópico: " + String(topico));
    debugInfo("Mensagem " + mensagem);

    debugInfo(String(strcmp(topico, TOPICO_COMANDO)));

    if (strcmp(topico, TOPICO_COMANDO) == 0)
    {
      tratarJsonComando(mensagem);
      return;
    }

    debugErro("Tópico näo tratado: " + String(topico));
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
      {
        SendUP = doc["telaRetratil"]["UP"].as<bool>();
      }
      if (doc["telaRetratil"]["DOWN"].is<bool>())
      {
        SendDOWN = doc["telaRetratil"]["DOWN"].as<bool>();
      }
      if (doc["telaRetratil"]["PAUSE"].is<bool>())
      {
        SendPAUSE = doc["telaRetratil"]["PAUSE"].as<bool>();
      }
      if (doc["telaRetratil"]["tela"].is<int8_t>())
      {
        Tela = doc["telaRetratil"]["tela"].as<int8_t>();
      }
    }
    Serial.println(SendUP);
    Serial.println(SendDOWN);
    Serial.println(SendPAUSE);

    enviarRF();
  }

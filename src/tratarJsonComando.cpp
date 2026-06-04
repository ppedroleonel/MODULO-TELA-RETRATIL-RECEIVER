//! TratarJsonComando.cpp
// Este arquivo processa comandos JSON recebidos via MQTT e os converte em ações RF
// para controlar as duas telas retráteis via protocolo de radiofrequência

#include <Arduino.h>
#include <ArduinoJson.h>
#include <DebugManager.h>
#include "tratarJsonComando.h"
#include "conectividade.h"
#include <TelaProjecaoRF.h>
#include "secrets.h"

// Flags de controle: indicam qual comando de movimento deve ser enviado para a tela
static bool SendUP = false;      // True = enviar comando para subir tela
static bool SendDOWN = false;    // True = enviar comando para baixar tela
static bool SendPAUSE = false;   // True = enviar comando para parar tela
static int8_t Tela = 0;          // Identificador da tela ativa (0 ou 1)

// Configuração dos pinos para comunicação RF com as telas
static const int PINO_TX = 7;    // Pino de transmissão
static const int PINO_RX = 6;    // Pino de recepção

// Instância do objeto de controle RF para as telas retráteis
static TelaProjecaoRF telaRF(PINO_TX, PINO_RX);

// Endereço RF da Tela 0 (5 bytes em hexadecimal)
static const uint8_t ENDERECO_TELA_0[TelaProjecaoRF::TAMANHO_ENDERECO] = {
    0xCD, 0x4E, 0x0A, 0x01, 0x00};

// Endereço RF da Tela 1 (5 bytes em hexadecimal)
static const uint8_t ENDERECO_TELA_1[TelaProjecaoRF::TAMANHO_ENDERECO] = {
    0xCD, 0x4B, 0xF6, 0x01, 0x00};

// Inicializa o módulo RF e configura a inversão de sinal (se necessário)
void inicializarRF()
{
  telaRF.begin(&Serial);
  telaRF.setInverterSinal(true);
}

// Atualiza o estado do módulo RF (chamado continuamente no loop)
void updateRF()
{
  telaRF.update();
}

// Envia os comandos RF para a tela selecionada
// Verifica qual tela está selecionada e envia o comando apropriado (UP, DOWN ou PAUSE)
void enviarRF()
{
  // Se Tela 0 está selecionada
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
  // Se Tela 1 está selecionada
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

// Callback chamado quando uma mensagem é recebida no tópico MQTT
// Verifica se o tópico é o esperado e processa a mensagem como comando JSON
void tratarMensagemRecebida(const char *topico, const String &mensagem)
{
    // Se a mensagem chegou no tópico de recepção correto
    if (strcmp(topico, rec[0]) == 0)
    {
        tratarJsonComando(mensagem);
    }
}

// Processa a mensagem JSON e extrai os comandos de controle
// Esperado formato: {"telaRetratil": {"UP": bool, "DOWN": bool, "PAUSE": bool, "tela": int}}
void tratarJsonComando(const String &mensagem)
{
    JsonDocument doc;

    // Desserializa a string JSON para um objeto
    DeserializationError erro = deserializeJson(doc, mensagem);

    // Se houve erro na desserialização, registra e retorna
    if (erro)
    {
      debugErro("Erro ao desserializar o JSON: " + String(erro.c_str()));
      return;
    }

    // Verifica se existe o objeto "telaRetratil" no JSON
    if (doc["telaRetratil"].is<JsonObject>())
    {
      // Extrai o comando UP (subir) se estiver presente
      if (doc["telaRetratil"]["UP"].is<bool>())
      {
        SendUP = doc["telaRetratil"]["UP"].as<bool>();
      }
      // Extrai o comando DOWN (baixar) se estiver presente
      if (doc["telaRetratil"]["DOWN"].is<bool>())
      {
        SendDOWN = doc["telaRetratil"]["DOWN"].as<bool>();
      }
      // Extrai o comando PAUSE (parar) se estiver presente
      if (doc["telaRetratil"]["PAUSE"].is<bool>())
      {
        SendPAUSE = doc["telaRetratil"]["PAUSE"].as<bool>();
      }
      // Extrai o ID da tela selecionada (0 ou 1)
      if (doc["telaRetratil"]["tela"].is<int8_t>())
      {
        Tela = doc["telaRetratil"]["tela"].as<int8_t>();
        Serial.println("Tela selecionada: " + String(Tela));
      }
    }

    // Envia o comando via RF para a tela selecionada
    enviarRF();
}
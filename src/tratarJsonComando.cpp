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
#include <ezTime.h>

// Variáveis de controle: indicam qual comando de movimento deve ser enviado para a tela
static int8_t message = 0; // Comando recebido do JSON (0: nenhum, 1: UP, 2: DOWN, 3: PAUSE)
static int8_t Tela = 0;    // Identificador da tela ativa (0 ou 1)

// Configuração dos pinos para comunicação RF com as telas
static const int PINO_TX = 9; // Pino de transmissão
static const int PINO_RX = 6; // Pino de recepção

// Inicialização do objeto de controle RF para as telas retráteis
static TelaProjecaoRF telaRF(PINO_TX, PINO_RX);

// Endereço RF da Tela 0
static const uint8_t ENDERECO_TELA_0[TelaProjecaoRF::TAMANHO_ENDERECO] = {
    0xCD, 0x4E, 0x0A, 0x01, 0x00};

// Endereço RF da Tela 1
static const uint8_t ENDERECO_TELA_1[TelaProjecaoRF::TAMANHO_ENDERECO] = {
    0xCD, 0x4B, 0xF6, 0x01, 0x00}; //CD 4B F6 01 00

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
    switch (message)
    {
    case 0:
      debugInfo(">> Subindo tela... 0");
      telaRF.enviarCima(ENDERECO_TELA_0);
      postarBotaoUp();
      break;
    case 1:
      debugInfo(">> Baixando tela... 0");
      telaRF.enviarBaixo(ENDERECO_TELA_0);
      postarBotaoDown();
      break;

    case 2:
      debugInfo(">> Parando tela... 0");
      telaRF.enviarParar(ENDERECO_TELA_0);
      postarBotaoPause();
    default:
      debugErro("Comando desconhecido recebido: " + String(message));
      break;
    }
  }
  // Se Tela 1 está selecionada
  else if (Tela == 1)
  {
    switch (message)
    {
    case 0:
      debugInfo(">> Subindo tela... 1");
      telaRF.enviarCima(ENDERECO_TELA_1);
      postarBotaoUp();
      break;
    case 1:
      debugInfo(">> Baixando tela... 1");
      telaRF.enviarBaixo(ENDERECO_TELA_1);
      postarBotaoDown();
      break;

    case 2:
      debugInfo(">> Parando tela... 1");
      telaRF.enviarParar(ENDERECO_TELA_1);
      postarBotaoPause();
    default:
      debugErro("Comando desconhecido recebido: " + String(message));
      break;
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
// Esperado formato: {"telaRetratil": {"tela": int, "UP": bool, "DOWN": bool, "PAUSE": bool}}
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
    // Extrai o ID da tela selecionada (0 ou 1)
    if (doc["telaRetratil"]["tela"].is<int8_t>())
    {
      Tela = doc["telaRetratil"]["tela"].as<int8_t>();
      debugInfo("Tela selecionada: " + String(Tela));
    }
    // Extrai o comando UP (subir) se estiver presente
    if (doc["telaRetratil"]["comando"].is<int8_t>())
    {
      message = doc["telaRetratil"]["comando"].as<int8_t>();
    }
  }

  // Envia o comando via RF para a tela selecionada
  enviarRF();
}

void postarBotaoDown()
{
  Timezone carimbo;

  waitForSync();

  carimbo.setLocation("America/Sao_Paulo");
  setInterval(60);

  JsonDocument doc;

  doc["telaRetratil"]["tela"] = Tela;
  doc["telaRetratil"]["info"] = "Tela Descendo";
  doc["telaRetratil"]["timestamp"] = carimbo.now();

  String texto;
  serializeJson(doc, texto);
  conexao.publicar(0, texto.c_str());
}

void postarBotaoUp()
{
  Timezone carimbo;

  waitForSync();

  carimbo.setLocation("America/Sao_Paulo");
  setInterval(60);

  JsonDocument doc;

  doc["telaRetratil"]["tela"] = Tela;
  doc["telaRetratil"]["info"] = "Tela subindo";
  doc["telaRetratil"]["timestamp"] = carimbo.now();

  String texto;
  serializeJson(doc, texto);
  conexao.publicar(0, texto.c_str());
}

void postarBotaoPause()
{
  Timezone carimbo;

  waitForSync();

  carimbo.setLocation("America/Sao_Paulo");
  setInterval(60);

  JsonDocument doc;

  doc["telaRetratil"]["tela"] = Tela;
  doc["telaRetratil"]["info"] = "Tela Pausada";
  doc["telaRetratil"]["timestamp"] = carimbo.now();

  String texto;
  serializeJson(doc, texto);
  conexao.publicar(0, texto.c_str());
}

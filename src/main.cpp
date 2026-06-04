/**
 *   Nome: Pedro Leonel de Lorena, Leonardo Ferrarese Correa, Lais Rodrigues Sevilhano & Luigi Arnosti Reginato
 *  Descrição: Neste modulo de publish nos coletamos os hex do controle remoto e fizemos com que toda vez que apertasse o botao ele emitsse para a tela oq o hex faz.
 *  Projeto: Modulo de publicar os comandos feito na tela retratil
 *  Data: 01/06/2026
 *  Versão: 1.0.2
 *  */

#include <Arduino.h>
#include <ESP32Connectivity.h>
#include <Conectividade.h>
#include <DebugManager.h>
#include <ArduinoJson.h>
#include <secrets.h>
#include <Bounce2.h>
#include <TelaProjecaoRF.h>
#include "tratarJsonComando.h"
#include "callbackManager.h"

ESP32Connectivity conexao;

// Inicializa a conexão Wi-Fi e MQTT, registra callbacks para eventos de conectividade,
// e inicia o módulo RF para captura de comandos do controle remoto
void setup()
{
  Serial.begin(115200);
  configurarDebug(DEBUG_NIVEL_INICIAL, PINO_HABILITAR_DEBUG_COMPLETO);
  conexao.beginAWS(wifiConfig, awsConfig, topicosConfig);
  conexao.registrarCallbackWiFiConectado(aoConectarWiFi);
  conexao.registrarCallbackWiFiDesconectado(aoDesconectarWiFi);
  conexao.registrarCallbackMQTTConectado(aoConectarMQTT);
  conexao.registrarCallbackMQTTDesconectado(aoDesconectarMQTT);
  conexao.registrarCallbackMensagem(tratarMensagemRecebida);
  inicializarRF();
}

void loop()
{
  conexao.update();
  updateRF();
}
/**
 *   Nome: Pedro Leonel de Lorena, Leonardo Ferrarese Correa, Lais Rodrigues Sevilhano & Luigi Arnosti Reginato
 *  Descrição: Neste módulo de receiver, o ESP32 recebe comandos JSON via MQTT e converte esses comandos em ações RF para controlar as telas retráteis, encaminhando UP, DOWN e PAUSE conforme o comando recebido.
 *  Projeto: Módulo de controle de tela retrátil via AWS IoT e RF
 *  Data: 04/06/2026
 *  Versão: 2.0.0
 *  */

#include <Arduino.h>
#include <ESP32Connectivity.h>
#include "Conectividade.h"
#include <DebugManager.h>
#include "secrets.h"
#include "tratarJsonComando.h"
#include "callbackManager.h"

// Inicialização do objeto de conectividade MQTT
 

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
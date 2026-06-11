//! tratarJsonComando.h
// Este arquivo declara as funções relacionadas ao tratamento de mensagens JSON recebidas via MQTT, incluindo a inicialização do controle RF e o processamento dos comandos para as telas retráteis.

#ifndef tratarJsonComando_h
#define tratarJsonComando_h
#include <Arduino.h>
#include <ArduinoJson.h>
#include <DebugManager.h>
#include "conectividade.h"
#include <TelaProjecaoRF.h>


void inicializarRF();
void updateRF();
void tratarJsonComando(const String &mensagem);
void tratarMensagemRecebida(const char *topico, const String &mensagem);
void enviarRF();
void postarBotaoDown();
void postarBotaoPause();
void postarBotaoUp();




#endif 
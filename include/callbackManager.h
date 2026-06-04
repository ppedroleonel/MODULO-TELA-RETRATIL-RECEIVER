// ! callbackManager.h
// Este arquivo declara as funções de callback para eventos de conectividade Wi-Fi e MQTT

#ifndef callbackManager_h
#define callbackManager_h

#include <Arduino.h>
#include <DebugManager.h>

void aoConectarWiFi();
void aoDesconectarWiFi();
void aoConectarMQTT();
void aoDesconectarMQTT();


#endif
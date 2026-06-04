//! callbackManager.cpp
// Este arquivo gerencia as funções de callback para eventos de conectividade
// Cada callback é acionado quando ocorre uma mudança no estado da conexão Wi-Fi ou MQTT

#include <Arduino.h>
#include "callbackManager.h"
#include <DebugManager.h>

// Callback acionado quando o Wi-Fi é conectado com sucesso
void aoConectarWiFi()
{
    debugInfo("WiFi conectado.");
}

// Callback acionado quando o Wi-Fi é desconectado
void aoDesconectarWiFi()
{
    debugAviso("WiFi desconectado.");
}

// Callback acionado quando a conexão MQTT com AWS IoT é estabelecida
void aoConectarMQTT()
{
    debugInfo("MQTT conectado.");
}

// Callback acionado quando a conexão MQTT com AWS IoT é perdida
void aoDesconectarMQTT()
{
    debugAviso("MQTT desconectado.");
}
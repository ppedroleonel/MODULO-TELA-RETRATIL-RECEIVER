//! conectividade.h
// Este arquivo declara as estruturas de configuração para Wi-Fi, AWS IoT e tópicos MQTT, além de externar as variáveis de configuração definidas em Conectividade.cpp

#ifndef conectividade_h
#define conectividade_h

#include <Arduino.h>
#include <ESP32Connectivity.h>

extern ConfigWiFi wifiConfig;
extern ConfigAWS awsConfig;
extern ConfigTopicos topicosConfig;
extern ESP32Connectivity conexao;

#endif
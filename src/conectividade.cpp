//! Conectividade.cpp
// Este arquivo configura as credenciais e parâmetros necessários para conectar ao AWS IoT, incluindo Wi-Fi, certificados e tópicos MQTT.

#include <Arduino.h>
#include <ESP32Connectivity.h>
#include "conectividade.h"
#include "secrets.h"
#include "tratarJsonComando.h"

// Configuração Wi-Fi: SSID e senha da rede
ConfigWiFi wifiConfig = {
    WIFI_SSID,
    WIFI_SENHA};

/* Configuração AWS IoT:
 - Endpoint do broker MQTT AWS
 - Porta 8883 para conexões seguras TLS
 - ID do cliente
 - Certificados CA, cliente e chave privada (inseridos do arquivo secrets.h)
 */
ConfigAWS awsConfig = {
    "a3cxr9ayydc4np-ats.iot.us-east-1.amazonaws.com",
    8883,
    "mario-1234",
    awsCertCA,
    awsCertCRT,
    awsCertPrivate};

/* Configuração dos tópicos MQTT:
  - pub: tópicos para publicar total de topicos 1
  - rec: tópicos para receber, total de topicos 1
  */
ConfigTopicos topicosConfig = {
    pub, 1,
    rec, 1};

ESP32Connectivity conexao;

//! Conectividade.cpp
// Este arquivo configura as credenciais e parâmetros necessários para conectar ao AWS IoT, incluindo Wi-Fi, certificados e tópicos MQTT.

#include <Arduino.h>
#include <ESP32Connectivity.h>
#include "conectividade.h"
#include "secrets.h"

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
    "a6t7fzch59vsw-ats.iot.us-east-1.amazonaws.com",
    8883,
    "telaRetratilPublisher",
    AWS_CERT_CA,      // era awsCertCA
    AWS_CERT_CRT,     // era awsCertCRT
    AWS_CERT_PRIVATE  // era awsCertPrivate
};

/* Configuração dos tópicos MQTT:
  - pub: tópicos para publicar total de topicos 1
  - rec: tópicos para receber, total de topicos 1
  */
ConfigTopicos topicosConfig = {
    TOPICOS_PUBLICAR, TOTAL_TOPICOS_PUBLICAR,
    TOPICOS_RECEBER, TOTAL_TOPICOS_RECEBER};
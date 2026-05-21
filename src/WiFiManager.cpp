//! WiFiManager.cpp
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "WiFiManager.h"
#include "secrets.h"
#include "DebugManager.h"


bool wifiEstaConectado()
{
    return WiFi.status() == WL_CONNECTED;
}

void conectarWiFi()
{
  debugInfo("========================");
  debugInfo("Iniciando conexão WiFi...");
  debugInfo("========================"); 

  WiFi.mode(WIFI_STA);    

  WiFi.begin(WIFI_SSID, WIFI_SENHA);
  
  debugInfo("Conectando");

  int tentativasWiFi = 0;
  const int maxTentativasWiFi = 30;

  while(WiFi.status() != WL_CONNECTED && tentativasWiFi < maxTentativasWiFi)   
  {
    delay(500);
    debugInfoSemLinha(".");                 
    tentativasWiFi++;     
  }

  debugInfo("");

  if(WiFi.status() == WL_CONNECTED)
  {
    debugInfo("WiFi conectado com sucesso!");
    debugInfoSemLinha("[INFO]Endereço IP: ");
    debugInfoSemLinha (WiFi.localIP().toString());
    debugInfo("\n\r");
  }
  
  else
  {
    debugErro("Falha ao conectar com o WiFi.");
    debugErro("Verifique SSID, senha e sinal de rede.");
  }
}


void garantirWiFiConectado()
{
  if(WiFi.status() != WL_CONNECTED)
  {
    debugErro("WiFi desconectado. Tentando reestabeceler a conexão...");
    conectarWiFi();
  }

  if(WiFi.status() != WL_CONNECTED)
  {
    debugErro("Não foi possivel reestabelecer a conexão");      
  }  
}
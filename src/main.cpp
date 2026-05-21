/**
 *   Nome: Pedro Leonel de Lorena, Leonardo Ferrarese Correa, Lais Rodrigues Sevilhano & Luigi Arnosti Reginato
 *  Descrição: Neste modulo de publish nos coletamos os hex do controle remoto e fizemos com que toda vez que apertasse o botao ele emitsse para a tela oq o hex faz.
 *  Projeto: Modulo de publicar os comandos feito na tela retratil
 *  Data: 21/05
 *  Versão: 0.0.1
 */

#include <Arduino.h>
#include "WiFiManager.h"
#include "MqttManager.h"
#include "DebugManager.h"
#include <ArduinoJson.h>
#include <secrets.h>
//=========================
//* PINOS
/*
    COLOCAR OS PINOS AQUI
*/
//=========================

//=========================
//* Variaveis globais
/*
    COLOCAR AS VARIAVIES GLOBAIS AQUI
*/
//=========================

//=========================
//* CRIAÇÃO DE OBJETOS 
/*
    COLOCAR OS OBJETOS AQUI
*/
//=========================

//=========================
//* INVOCAR AS FUNÇÕES
/*
    INVOCAR AS FUNÇõES AQUI
*/
//=========================



void setup()
{
  //=========================
  //* pinMODE
  /*
    COLOCAR OS pinMode AQUI
  */
  //=========================

  configurarDebug();
  conectarWiFi();
  configurarMQTT();
  conectarMQTT();
}

void loop()
{
  garantirWiFiConectado();
  garantirMQTTConectado();
  loopMQTT();
}

void postarJson()
{
  

  // esse eh a ultima parte pra enviar ou seja tem q codar a cima 
  // se ficar com duvida ve o projeto do esp 1
  String texto;
  serializeJson(doc, texto);
  publicarMensagem(TOPICO_COMANDO, texto.c_str());
}


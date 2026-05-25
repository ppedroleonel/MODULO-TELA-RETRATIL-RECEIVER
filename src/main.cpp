/**
 *   Nome: Pedro Leonel de Lorena, Leonardo Ferrarese Correa, Lais Rodrigues Sevilhano & Luigi Arnosti Reginato
 *  Descrição: Neste modulo de publish nos coletamos os hex do controle remoto e fizemos com que toda vez que apertasse o botao ele emitsse para a tela oq o hex faz.
 *  Projeto: Modulo de publicar os comandos feito na tela retratil
 *  Data: 21/05
 *  Versão: 0.0.2
 */

#include <Arduino.h>
#include "WiFiManager.h"
#include "MqttManager.h"
#include "DebugManager.h"
#include <ArduinoJson.h>
#include <secrets.h>
#include <Bounce2.h>
//=========================
//* PINOS
/*
    COLOCAR OS PINOS AQUI
*/
//=========================

//=========================
//* Variaveis globais

String hexUP = "codigo do hexUP";
String hexDOWN = "codigo do hexDOWN";
String hexSELECT = "codigo do hexSELECT";



//=========================

//=========================
//* CRIAÇÃO DE OBJETOS 
  
Bounce UP = Bounce();
Bounce DOWN = Bounce();
Bounce SELECT = Bounce();
//=========================

//=========================
//* INVOCAR AS FUNÇÕES
 
bool botaoUP();
bool botaoDOWN();
bool botaoSELECT();

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

  if(UP.fell())
  {
    botaoUP();
  }
}

bool botaoUP()
{ 
  if(hexUP == "")
    debugErro("O código do hex está errado, favor checar se esta correto");
 

if(hexUP)
{
  debugInfo("Tela retrátil subindo");
  //TODO VER QUANTO TEMPO ELA DEMORA PRA SUBIR POR COMPLETO E FAZER UM CRONOMETTRO Q QUANDO ELA SUBIR TUDO VAI EXIVIR
  //TODO FAZER UMA VARIAVEL DE PAUSE Q SALVA AONDE O MILLIS PAROU POIS QND ELE APERTAR O PAUSE O MILLIS NAO PODE CONTINUAR ANDANDO 
  if(millis() - 0 >= 5000 ) // trocar esse 5000 dps
  debugInfo("Tela retrátil subiu por completo.");


  String texto;
  serializeJson(doc, texto);  
  publicarMensagem(TOPICO_COMANDO, texto.c_str()); // ver como faz pra trocar o topico_comando pelo topico_publicar


}
}

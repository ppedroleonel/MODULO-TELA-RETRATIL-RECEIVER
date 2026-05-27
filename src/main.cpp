/**
 *   Nome: Pedro Leonel de Lorena, Leonardo Ferrarese Correa, Lais Rodrigues Sevilhano & Luigi Arnosti Reginato
 *  Descrição: Neste modulo de publish nos coletamos os hex do controle remoto e fizemos com que toda vez que apertasse o botao ele emitsse para a tela oq o hex faz.
 *  Projeto: Modulo de publicar os comandos feito na tela retratil
 *  Data: 21/05
 *  Versão: 0.0.5
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

const int pinUp = 2;
const int pinDown = 3;
const int pinPause = 4;

//=========================

//=========================
//* Variaveis globais

String frequenciaUp = "frequenciaUP";
String frequenciaDown = "frequenciaDOWN";
String frequenciaPause = "frequenciaPAUSE";

const unsigned long tempoInicio = 0;
unsigned long tempo = 0;
unsigned long tempoDecorrido = 0;
unsigned long tempoPause;
unsigned long tempoMaximo = 20000;
bool tempoParado = false;
//=========================

//=========================
//* CRIAÇÃO DE OBJETOS

Bounce UP = Bounce();
Bounce DOWN = Bounce();
Bounce PAUSE = Bounce();
//=========================

//=========================
//* INVOCAR AS FUNÇÕES

void botaoUp();
void botaoDown();
void botaoPause();

//=========================

void setup()
{
  //=========================
  //* pinMODE
  pinMode(pinUp, INPUT_PULLUP);
  pinMode(pinDown, INPUT_PULLUP); 
  pinMode(pinPause, INPUT_PULLUP);
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

  if (UP.fell())
  {
    botaoUp();
  }
  if (DOWN.fell())
  {
    botaoDown();
  }
  if (PAUSE.fell())
  {
    botaoPause();
  }
  
}


/*
  A ideia é que o botaoUP e down tenha a mesma logica pois ambos tem a mesma função, só que um sobe e o outro desce, e o botaoPAUSE tem a função de pausar o tempo, ou seja, quando ele for apertado ele salva o tempo decorrido e para de contar, e quando ele for apertado novamente ele continua contando de onde parou.
*/
void botaoUp()
{
  if (frequenciaUp == "")
    debugErro("A frequência do botão UP está errada, favor checar se está correta");

  if (frequenciaUp == "frequencia do botão UP")
  {
    if (tempoParado)
    {
      debugInfo("Tela retrátil subindo");
      tempo = millis() - tempoPause;
      tempoParado = false;

      // Serial para debug
    debugInfo("Tempo decorrido: " + String(tempoDecorrido) + " ms");
    }
    tempoDecorrido = millis() - tempo;

    if (tempoDecorrido>= tempoMaximo)
    {
      debugInfo("Tela retrátil subiu por completo.");
      tempoDecorrido = 0;
      tempoParado = false;
    }
  }
  //TODO: PUBLICAR O JSON DO BOTAO UP NO MQTT

}

void botaoPause()
{
  if (frequenciaPause == "")
    debugErro("A frequência do botão Pause está errada, favor checar se está correta");

  if (frequenciaPause == "frequencia do botão PAUSE")
  {
    debugInfo("Tela retrátil pausada");
    tempoParado = true;
    tempoPause = tempoDecorrido;

  }
  //TODO: PUBLICAR O JSON DO BOTAO PAUSE NO MQTT
}

void botaoDown()
{
  if (frequenciaDown == "")
    debugErro("A frequência do botão DOWN está errada, favor checar se está correta");

  if (frequenciaDown == "frequencia do botão DOWN")
  {
    if (tempoParado)
    {
      debugInfo("Tela retrátil descendo");
      tempo = millis() - tempoPause;
      tempoParado = false;

       // Serial para debug
    debugInfo("PAUSADO em: " + String(tempoPause) + " ms");
    }
    tempoDecorrido = millis() - tempo;

    if (tempoDecorrido >= tempoMaximo)
    {
      debugInfo("Tela retrátil desceu por completo.");
      tempoDecorrido = 0;
      tempoParado = false;
    }
  }
  //TODO: PUBLICAR O JSON DO BOTAO DOWN NO MQTT
}
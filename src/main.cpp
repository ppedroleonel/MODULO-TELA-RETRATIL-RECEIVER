/**
 *   Nome: Pedro Leonel de Lorena, Leonardo Ferrarese Correa, Lais Rodrigues Sevilhano & Luigi Arnosti Reginato
 *  Descrição: Neste modulo de publish nos coletamos os hex do controle remoto e fizemos com que toda vez que apertasse o botao ele emitsse para a tela oq o hex faz.
 *  Projeto: Modulo de publicar os comandos feito na tela retratil
 *  Data: 21/05
 *  Versão: 0.0.4
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

String frequenciaUP = "frequenciaUP";
String frequenciaDOWN = "frequenciaDOWN";
String frequenciaPAUSE = "frequenciaPAUSE";

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
Bounce SELECT = Bounce();
//=========================

//=========================
//* INVOCAR AS FUNÇÕES

void botaoUP();
void botaoDOWN();
void botaoSELECT();

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

  if (UP.fell())
  {
    botaoUP();
  }
}


/*
  A ideia é que o botaoUP e down tenha a mesma logica pois ambos tem a mesma função, só que um sobe e o outro desce, e o botaoPAUSE tem a função de pausar o tempo, ou seja, quando ele for apertado ele salva o tempo decorrido e para de contar, e quando ele for apertado novamente ele continua contando de onde parou.
*/
void botaoUP()
{
  if (frequenciaUP == "")
    debugErro("A frequência do botão UP está errada, favor checar se está correta");

  if (frequenciaUP == "frequencia do botão UP")
  {
    // TODO VER QUANTO TEMPO ELA DEMORA PRA SUBIR POR COMPLETO E FAZER UM CRONOMETTRO Q QUANDO ELA SUBIR TUDO VAI EXIVIR
    // TODO FAZER UMA VARIAVEL DE PAUSE Q SALVA AONDE O MILLIS PAROU POIS QND ELE APERTAR O PAUSE O MILLIS NAO PODE CONTINUAR ANDANDO
    if (tempoParado)
    {
      debugInfo("Tela retrátil subindo");
      tempo = millis() - tempoPause;
      tempoParado = false;

      // Serial para debug
    debugInfo("Tempo decorrido: " + String(tempoDecorrido));
    debugInfo(" ms");
    }
    tempoDecorrido = millis() - tempo;

    if (tempoDecorrido>= tempoMaximo)
    {
      debugInfo("Tela retrátil subiu por completo.");
      tempoDecorrido = 0;
      tempoParado = false;
    }
  }
}

void botaoPause()
{
  if (frequenciaPAUSE == "")
    debugErro("A frequência do botão Pause está errada, favor checar se está correta");

  if (frequenciaPAUSE == "frequencia do botão PAUSE")
  {
    debugInfo("Tela retrátil pausada");
    tempoParado = true;
    tempoPause = tempoDecorrido;

  }
}

void botaoDOWN()
{
  if (frequenciaDOWN == "")
    debugErro("A frequência do botão DOWN está errada, favor checar se está correta");

  if (frequenciaDOWN == "frequencia do botão DOWN")
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
}
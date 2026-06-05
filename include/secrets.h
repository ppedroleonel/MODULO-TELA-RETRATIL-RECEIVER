//! secrets.h
// Este arquivo armazena as credenciais e configurações sensíveis, como SSID, senha Wi-Fi, certificados AWS IoT e tópicos MQTT. Ele é incluído em outros arquivos para acessar essas informações de forma centralizada.

#ifndef SECRETS_H
#define SECRETS_H

extern const char *WIFI_SSID;
extern const char *WIFI_SENHA;


extern const char AWS_CERT_CA[];
extern const char AWS_CERT_CRT[];
extern const char AWS_CERT_PRIVATE[];
extern const char *TOPICOS_PUBLICAR[];
extern const char *TOPICOS_RECEBER[];
extern const int TOTAL_TOPICOS_PUBLICAR;
extern const int TOTAL_TOPICOS_RECEBER;

//======================================
// *DEBUG
//======================================

/*
0	DEBUG_NENHUM | Nada
1	DEBUG_ERRO	| Falhas críticas
2	DEBUG_AVISO	| Reconexões e timeouts
3	DEBUG_INFO	| Fluxo normal
4	DEBUG_VERBOSE	| Tentativas e estados internos
5	DEBUG_TUDO | Payloads e códigos de erro brutos
*/
extern const int DEBUG_NIVEL_INICIAL;

// Pino usado para forçar todas as mensagens na inicialização
extern const int PINO_HABILITAR_DEBUG_COMPLETO;

#endif
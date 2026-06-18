
# 🎬 Controle de Telas Retráteis · MQTT + RF  
### *Transforme comandos da nuvem em movimento — sem fio, sem botão.*

[![PlatformIO](https://img.shields.io/badge/PlatformIO-ESP32-orange?logo=platformio)](https://platformio.org/)
[![ESP32](https://img.shields.io/badge/ESP32-Espressif-blue?logo=espressif)](https://www.espressif.com/)
[![AWS IoT](https://img.shields.io/badge/AWS_IoT_Core-MQTT-orange?logo=amazon-aws)](https://aws.amazon.com/iot-core/)
[![RF 433MHz](https://img.shields.io/badge/RF-433MHz-lightgrey)](#)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Status](https://img.shields.io/badge/status-active-brightgreen)](#)

---

## 📖 Sobre o projeto  

Imagine uma sala de reunião, um home theater ou uma escola. Com esse sistema, **você controla telas de projeção automaticamente pelo celular ou pela internet** — sem precisar apertar nenhum botão físico.

O comando viaja da nuvem (AWS IoT) até um microcontrolador ESP32 e, de lá, é enviado por **radiofrequência (433 MHz)** diretamente ao motor da tela, como um controle remoto original.

Tudo **não-bloqueante**, com reconexão automática e fila offline — cortesia da biblioteca [ESP32Connectivity](https://github.com/professorThiago/ESP32Connectivity).

---

## ✨ Funcionalidades  

- ✅ **Comando remoto via MQTT** – publique um JSON e a tela se move.  
- ✅ **Autenticação mútua TLS** – certificados de dispositivo para AWS IoT Core.  
- ✅ **Dois canais independentes** – controle até 2 telas com endereços RF distintos.  
- ✅ **Três comandos por tela** – subir (`UP`), descer (`DOWN`) e parar (`PAUSE`).  
- ✅ **Log configurável** – 5 níveis de verbosidade com `DebugManager`.  
- ✅ **Callbacks de conectividade** – saiba exatamente quando WiFi ou MQTT conectam/desconectam.  
- ✅ **Fila offline** (via ESP32Connectivity) – comandos não são perdidos se a rede cair.  

---

## 📦 Estrutura do projeto  

| Arquivo | Responsabilidade |
|---------|------------------|
| `secrets.h` | Cofre de credenciais: WiFi, certificados AWS, tópicos MQTT e debug |
| `conectividade.h/.cpp` | Structs de configuração (`ConfigWiFi`, `ConfigAWS`, `ConfigTopicos`) |
| `callbackManager.h/.cpp` | Callbacks disparadas em eventos de conexão Wi‑Fi e MQTT |
| `tratarJsonComando.h/.cpp` | Desserializa o JSON, decide qual tela/comando e aciona o transmissor RF |
| `main.ino` / `main.cpp` | `setup()` e `loop()` — inicializa tudo e mantém o sistema rodando |

---

## ⚙️ Configuração  

### 1. Credenciais e certificados (`secrets.h`)  

```cpp
const char *WIFI_SSID = "MinhaRede";
const char *WIFI_SENHA = "SenhaWiFi";
const char *TOPICOS_RECEBER[] = {"telas/comando"};
const int TOTAL_TOPICOS_RECEBER = 1;

const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
...
-----END CERTIFICATE-----
)EOF";
// AWS_CERT_CRT[] e AWS_CERT_PRIVATE[] seguem o mesmo formato

const int DEBUG_NIVEL_INICIAL = DEBUG_INFO;   // 0=Nada, 5=Tudo
const int PINO_HABILITAR_DEBUG_COMPLETO = 5;
```

### 2. Endereços RF das telas (`tratarJsonComando.cpp`)  

```cpp
static const int PINO_TX = 7;
static const int PINO_RX = 6;

static const uint8_t ENDERECO_TELA_0[] = {0xCD, 0x4E, 0x0A, 0x01, 0x00};
static const uint8_t ENDERECO_TELA_1[] = {0xCD, 0x4B, 0xF6, 0x01, 0x00};
```

> 💡 *Descubra os endereços corretos capturando o sinal do controle original com um sniffer RF.*

---

## 📨 Formato da mensagem JSON  

Publique no tópico configurado um objeto como este:

```json
{
  "telaRetratil" : {
    "tela": 0,
    "comando": 1
  }
}
```

| Campo | Tipo | Descrição |
|-------|------|-----------|
| `tela` | `int` | `0` para tela 1, `1` para tela 2 |
| `comando` | `int` | `0` para subir, `1` para descer, `2` para tela pausar|

---

## 🚀 Uso  

1. Clone este repositório.  
2. Instale as dependências (via PlatformIO ou manualmente).  
3. Preencha `secrets.h` com suas credenciais e certificados.  
4. Ajuste os endereços RF das telas em `tratarJsonComando.cpp`.  
5. Compile e grave no ESP32.  
6. Publique um JSON no tópico `telas/comando` usando o AWS IoT MQTT test client ou qualquer app MQTT.  

---

## 🧰 Dependências  

| Biblioteca | Versão | Descrição |
|------------|--------|-----------|
| [ESP32Connectivity](https://github.com/professorThiago/ESP32Connectivity) | 3.0.0 | WiFi + MQTT não‑bloqueante com fila offline |
| [DebugManager](https://github.com/professorThiago/DebugManager) | ≥1.0.0 | Log colorido e configurável via pinos |
| [ArduinoJson](https://arduinojson.org/) | 7.2.2 | Parse de JSON em sistemas embarcados |
| [TelaProjecaoRF] (https://github.com/professorThiago/TelaProjecaoRF) | 1.1.0  | Classe para comunicação RF com telas retráteis |


---

## 🎛️ Níveis de debug  

Defina `DEBUG_NIVEL_INICIAL` em `secrets.h`:

| Nível | Constante | Exibe |
|-------|-----------|-------|
| 0 | `DEBUG_NENHUM` | Nada |
| 1 | `DEBUG_ERRO` | Falhas críticas |
| 2 | `DEBUG_AVISO` | Reconexões, timeouts |
| 3 | `DEBUG_INFO` | Fluxo normal (conexões, comandos executados) |
| 4 | `DEBUG_VERBOSE` | Estados internos e tentativas |
| 5 | `DEBUG_TUDO` | Payloads completos e códigos de erro |

---

## 👥 Autores  

| Nome | GitHub |
|------|--------|
| Luigi Reginato | [@luigi-reginato](https://github.com/luigi-reginato) |
| Pedro Leonel | [@ppedroleonel](https://github.com/ppedroleonel) |
| Leonardo Ferrarese | [@o-lj](https://github.com/O-LJ) |
| Lais Rodrigues | [@LaisSevilhano90](https://github.com/LaisSevilhano90) |


---


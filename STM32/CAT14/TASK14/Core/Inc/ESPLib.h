/* ESP8266 polling transport and bounded MQTT 3.1.1 QoS 0 client.
 * Interface adapted from ESPLib by Can Guveren (2022); see docs/ATTRIBUTION.md.
 */
#ifndef INC_ESPLIB_H_
#define INC_ESPLIB_H_
#include "stm32f4xx_hal.h"
#include <stdint.h>
typedef enum { funcErr, funcOk } funcState_t;
typedef enum { StationMode=1, AccessPointMode, StationAP } espMode_t;
typedef struct {
 char *clientID, *username, *password;
 uint16_t keepAlive;
 uint8_t connectFlag;
} MQTT_InitTypeDef_t;
#define CleanSession 0x02
#define QoS0 0
funcState_t ESP8266_Init(espMode_t mode, UART_HandleTypeDef *uart);
funcState_t ESP8266_wifiConnect(const char *ssid, const char *password);
funcState_t ESP8266_portConnect(const char *type, const char *host, const char *port);
funcState_t MQTT_connectBroker(const MQTT_InitTypeDef_t *config);
funcState_t MQTT_publishTopic(const char *topic, const char *message, uint8_t qos);
#endif

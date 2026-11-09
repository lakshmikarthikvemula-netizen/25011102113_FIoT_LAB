#ifndef ESP_AT_H
#define ESP_AT_H

#include "main.h"
#include <stddef.h>

typedef void (*EspLogByte)(uint8_t byte);
typedef int (*EspHttpReply)(char *body, size_t capacity);

int esp_command(UART_HandleTypeDef *uart, const char *command, const char *reply,
                uint32_t timeout, EspLogByte log_byte);
int esp_join(UART_HandleTypeDef *uart, const char *ssid, const char *password);
int esp_http_start(UART_HandleTypeDef *uart);
void esp_http_poll(UART_HandleTypeDef *uart, const char *content_type, EspHttpReply make_body);

#endif

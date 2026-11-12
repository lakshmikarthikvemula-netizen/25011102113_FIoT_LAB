/* Generated blocking UART helper for ESP8266 AT firmware. */
#include "esp_at.h"
#include <stdio.h>
#include <string.h>

static int send(UART_HandleTypeDef *uart, const char *data)
{
    return HAL_UART_Transmit(uart, (const uint8_t *)data, strlen(data), 1000) == HAL_OK;
}

static int wait_for(UART_HandleTypeDef *uart, const char *reply, uint32_t timeout,
                    EspLogByte log_byte)
{
    size_t matched = 0;
    uint32_t started = HAL_GetTick();
    while (HAL_GetTick() - started < timeout) {
        uint8_t byte;
        if (HAL_UART_Receive(uart, &byte, 1, 10) != HAL_OK)
            continue;
        if (log_byte)
            log_byte(byte);
        if (byte == (uint8_t)reply[matched]) {
            if (!reply[++matched])
                return 1;
        } else {
            matched = byte == (uint8_t)reply[0] ? 1u : 0u;
        }
    }
    return 0;
}

int esp_command(UART_HandleTypeDef *uart, const char *command, const char *reply,
                uint32_t timeout, EspLogByte log_byte)
{
    char line[192];
    int n = snprintf(line, sizeof(line), "%s\r\n", command);
    return n > 0 && (size_t)n < sizeof(line) && send(uart, line) &&
           wait_for(uart, reply, timeout, log_byte);
}

int esp_join(UART_HandleTypeDef *uart, const char *ssid, const char *password)
{
    char command[160];
    const char *ok = "OK\r\n";
    if (strpbrk(ssid, "\"\\,\r\n") || strpbrk(password, "\"\\,\r\n"))
        return 0;
    int n = snprintf(command, sizeof(command), "AT+CWJAP=\"%s\",\"%s\"", ssid, password);
    return n > 0 && (size_t)n < sizeof(command) &&
           esp_command(uart, "AT", ok, 2000, NULL) &&
           esp_command(uart, "ATE0", ok, 2000, NULL) &&
           esp_command(uart, "AT+CWMODE=1", ok, 2000, NULL) &&
           esp_command(uart, command, ok, 25000, NULL);
}

int esp_http_start(UART_HandleTypeDef *uart)
{
    const char *ok = "OK\r\n";
    return esp_command(uart, "AT+CIPMUX=1", ok, 2000, NULL) &&
           esp_command(uart, "AT+CIPSERVER=1,80", ok, 3000, NULL);
}

void esp_http_poll(UART_HandleTypeDef *uart, const char *content_type, EspHttpReply make_body)
{
    char header[24], command[40], body[192], response[360], method[4];
    size_t used = 0;
    unsigned link, length;
    char extra;
    uint8_t byte;
    uint32_t started;
    if (!wait_for(uart, "+IPD,", 100, NULL))
        return;
    started = HAL_GetTick();
    do {
        if (used == sizeof(header) - 1 || HAL_GetTick() - started >= 500 ||
            HAL_UART_Receive(uart, &byte, 1, 50) != HAL_OK)
            return;
        if (byte != ':')
            header[used++] = (char)byte;
    } while (byte != ':');
    header[used] = 0;
    if (sscanf(header, "%u,%u%c", &link, &length, &extra) != 2 || link > 4)
        return;
    started = HAL_GetTick();
    for (unsigned i = 0; i < length; ++i) {
        if (HAL_GetTick() - started >= 1500 ||
            HAL_UART_Receive(uart, &byte, 1, 100) != HAL_OK)
            return;
        if (i < sizeof(method))
            method[i] = (char)byte;
    }
    if (length < sizeof(method) || memcmp(method, "GET ", sizeof(method)) != 0)
        return;

    int count = make_body(body, sizeof(body));
    const char *status = "200 OK";
    if (count < 0 || (size_t)count >= sizeof(body)) {
        status = "503 Service Unavailable";
        count = snprintf(body, sizeof(body), "{\"status\":\"sensor unavailable\"}");
        content_type = "application/json";
    }
    int n = snprintf(response, sizeof(response),
                     "HTTP/1.1 %s\r\nContent-Type: %s\r\nContent-Length: %d\r\n"
                     "Connection: close\r\n\r\n%s", status, content_type, count, body);
    if (n > 0 && (size_t)n < sizeof(response)) {
        snprintf(command, sizeof(command), "AT+CIPSEND=%u,%d", link, n);
        if (esp_command(uart, command, ">", 3000, NULL) && send(uart, response))
            wait_for(uart, "SEND OK", 5000, NULL);
    }
    snprintf(command, sizeof(command), "AT+CIPCLOSE=%u", link);
    esp_command(uart, command, "OK\r\n", 2000, NULL);
}

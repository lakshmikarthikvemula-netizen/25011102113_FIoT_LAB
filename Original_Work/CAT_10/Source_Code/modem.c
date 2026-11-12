/* Generated lab example. Call modem_run after initializing both UARTs. */
#include "main.h"
#include "esp_at.h"
#include <stdio.h>
#include <string.h>

extern UART_HandleTypeDef huart1, huart2;
#ifndef WIFI_SSID
#define WIFI_SSID "YOUR_SSID"
#define WIFI_PASSWORD "YOUR_PASSWORD"
#endif

static void log_byte(uint8_t byte)
{
    HAL_UART_Transmit(&huart2, &byte, 1, 100);
}

static void log_line(const char *line)
{
    HAL_UART_Transmit(&huart2, (const uint8_t *)line, strlen(line), 100);
}

void modem_run(void)
{
    static const char *commands[] = {"AT", "AT+GMR", "AT+CWMODE=1", "AT+CWLAP"};
    for (unsigned i = 0; i < sizeof(commands) / sizeof(commands[0]); ++i) {
        log_line(commands[i]);
        log_line("\r\n");
        if (!esp_command(&huart1, commands[i], "\r\nOK\r\n", 15000, log_byte)) {
            log_line("AT command failed\r\n");
            return;
        }
    }
    if (strcmp(WIFI_SSID, "YOUR_SSID") == 0) {
        log_line("Set Wi-Fi credentials locally to run the station test.\r\n");
        return;
    }
    if (esp_join(&huart1, WIFI_SSID, WIFI_PASSWORD) &&
        esp_command(&huart1, "AT+CIFSR", "\r\nOK\r\n", 3000, log_byte))
        log_line("WIFI MODE: STATION\r\nWIFI STATUS: CONNECTED\r\n");
    else
        log_line("Wi-Fi setup failed\r\n");
}

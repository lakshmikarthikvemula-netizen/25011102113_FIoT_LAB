/* Generated lab example. Call bme_http_run after CubeMX initialization. */
#include "bme_lab.h"
#include "esp_at.h"

extern UART_HandleTypeDef huart1;
#ifdef LAB_DEBUG_UART
extern UART_HandleTypeDef huart2;
#endif

#ifndef LAB_SENSOR_ONLY
#define LAB_SENSOR_ONLY 0
#endif

static int sample(char *body, size_t capacity)
{
    int length = bme_lab_json(body, capacity);
#ifdef LAB_DEBUG_UART
    if (length > 0 && (size_t)length < capacity) {
        const uint8_t newline[] = "\r\n";
        HAL_UART_Transmit(&huart2, (const uint8_t *)body, length, 100);
        HAL_UART_Transmit(&huart2, newline, sizeof(newline) - 1, 100);
    }
#endif
    return length;
}

void bme_http_run(void)
{
    if (!bme_lab_start()) {
        Error_Handler();
        return;
    }
    if (LAB_SENSOR_ONLY) {
        char body[160];
        for (;;) {
            sample(body, sizeof(body));
            HAL_Delay(1100);
        }
    }
    /* The ESP8266 must already be connected to the access point. */
    if (!esp_http_start(&huart1)) {
        Error_Handler();
        return;
    }
    for (;;)
        esp_http_poll(&huart1, "application/json", sample);
}

/* Generated lab example. Call uart_led_run after CubeMX initialization. */
#include "main.h"
#include <string.h>

extern UART_HandleTypeDef huart1;
#ifndef LAB_MODE
#define LAB_MODE 2  /* 0 = transmit, 1 = echo, 2 = LED commands */
#endif
#ifndef LED_ON
#define LED_ON GPIO_PIN_SET
#endif

static void send(const char *text)
{
    HAL_UART_Transmit(&huart1, (const uint8_t *)text, strlen(text), 100);
}

void uart_led_run(void)
{
    char line[32];
    size_t length = 0;
    int overflow = 0;
    const GPIO_PinState off = LED_ON == GPIO_PIN_SET ? GPIO_PIN_RESET : GPIO_PIN_SET;

    for (;;) {
        uint8_t byte;
        if (LAB_MODE == 0) {
            send("A");
            HAL_Delay(500);
            continue;
        }
        if (HAL_UART_Receive(&huart1, &byte, 1, HAL_MAX_DELAY) != HAL_OK)
            continue;
        if (LAB_MODE == 1) {
            HAL_UART_Transmit(&huart1, &byte, 1, 100);
            continue;
        }
        if (byte != '\r' && byte != '\n') {
            if (length < sizeof(line) - 1 && byte != 0)
                line[length++] = (char)byte;
            else
                overflow = 1;
            continue;
        }
        if (length || overflow) {
            line[length] = 0;
            if (!overflow && strcmp(line, "LED ON") == 0) {
                HAL_GPIO_WritePin(LAB_LED_GPIO_Port, LAB_LED_Pin, LED_ON);
                send("LED turned ON\r\n");
            } else if (!overflow && strcmp(line, "LED OFF") == 0) {
                HAL_GPIO_WritePin(LAB_LED_GPIO_Port, LAB_LED_Pin, off);
                send("LED turned OFF\r\n");
            } else if (!overflow && strcmp(line, "LED TOGGLE") == 0) {
                HAL_GPIO_TogglePin(LAB_LED_GPIO_Port, LAB_LED_Pin);
                send("LED toggled\r\n");
            } else {
                send("Invalid command\r\n");
            }
        }
        length = 0;
        overflow = 0;
    }
}

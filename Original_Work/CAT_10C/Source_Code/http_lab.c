/* Generated lab example. Call http_lab_run after MX_USART1_UART_Init(). */
#include "esp_at.h"
#include <stdio.h>

extern UART_HandleTypeDef huart1;

static int make_body(char *body, size_t capacity)
{
    return snprintf(body, capacity, "<html><body><h1>STM32 IoT Node</h1>"
                    "<p>Hello from STM32</p></body></html>");
}

void http_lab_run(void)
{
    /* Join Wi-Fi first using CAT 10 or issue esp_join here with local credentials. */
    if (!esp_command(&huart1, "AT", "OK\r\n", 2000, NULL) ||
        !esp_http_start(&huart1))
        { Error_Handler(); return; }
    for (;;)
        esp_http_poll(&huart1, "text/html", make_body);
}

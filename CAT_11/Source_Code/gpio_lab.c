/* Generated lab example. Call gpio_lab_run after MX_GPIO_Init(). */
#include "main.h"

#ifndef LAB_TASK
#define LAB_TASK 3
#endif

static int pressed(void)
{
    return HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET;
}

void gpio_lab_run(void)
{
    int stable = pressed();
    int candidate = stable;
    uint32_t changed = HAL_GetTick();
    uint32_t started = changed;
    for (;;) {
        if (LAB_TASK == 1) {
            HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
            HAL_Delay(500);
            continue;
        }
        uint32_t now = HAL_GetTick();
        int raw = pressed();
        if (raw != candidate) {
            candidate = raw;
            changed = now;
        }
        if (candidate != stable && now - changed >= 30) {
            stable = candidate;
            if (stable) {
                started = now;
                if (LAB_TASK == 2)
                    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
            } else if (LAB_TASK == 3) {
                if (now - started < 1000)
                    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
                else
                    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
            }
        }
        HAL_Delay(5);
    }
}

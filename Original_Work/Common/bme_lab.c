/* Generated adapter for the Bosch BME280 SensorAPI (three-argument settings API). */
#include "main.h"
#include "bme280.h"
#include "bme_lab.h"
#include <stdio.h>

#ifndef BME280_DOUBLE_ENABLE
#error Use the Bosch driver with double compensation enabled.
#endif
#ifndef LAB_BME_ADDRESS
#define LAB_BME_ADDRESS BME280_I2C_ADDR_PRIM
#endif

extern I2C_HandleTypeDef hi2c1;
static uint8_t address = LAB_BME_ADDRESS;
static struct bme280_dev sensor;

/* Watch these variables in the debugger. */
volatile uint8_t lab_chip_id;
volatile double lab_temperature, lab_pressure, lab_humidity;

static BME280_INTF_RET_TYPE read_reg(uint8_t reg, uint8_t *data, uint32_t size, void *context)
{
    uint16_t device = (uint16_t)(*(uint8_t *)context << 1);
    return HAL_I2C_Mem_Read(&hi2c1, device, reg, I2C_MEMADD_SIZE_8BIT, data, size, 100)
           == HAL_OK ? BME280_OK : BME280_E_COMM_FAIL;
}

static BME280_INTF_RET_TYPE write_reg(uint8_t reg, const uint8_t *data,
                                      uint32_t size, void *context)
{
    uint16_t device = (uint16_t)(*(uint8_t *)context << 1);
    return HAL_I2C_Mem_Write(&hi2c1, device, reg, I2C_MEMADD_SIZE_8BIT,
                             (uint8_t *)data, size, 100)
           == HAL_OK ? BME280_OK : BME280_E_COMM_FAIL;
}

static void delay_us(uint32_t period, void *context)
{
    (void)context;
    HAL_Delay(period / 1000 + (period % 1000 != 0));
}

int bme_lab_start(void)
{
    struct bme280_settings settings = {
        .osr_t = BME280_OVERSAMPLING_1X,
        .osr_p = BME280_OVERSAMPLING_1X,
        .osr_h = BME280_OVERSAMPLING_1X,
        .filter = BME280_FILTER_COEFF_OFF,
        .standby_time = BME280_STANDBY_TIME_1000_MS
    };
    sensor.intf = BME280_I2C_INTF;
    sensor.intf_ptr = &address;
    sensor.read = read_reg;
    sensor.write = write_reg;
    sensor.delay_us = delay_us;
    if (bme280_init(&sensor) != BME280_OK)
        return 0;
    lab_chip_id = sensor.chip_id;
    if (bme280_set_sensor_settings(BME280_SEL_ALL_SETTINGS, &settings, &sensor) != BME280_OK ||
        bme280_set_sensor_mode(BME280_POWERMODE_NORMAL, &sensor) != BME280_OK)
        return 0;
    HAL_Delay(20);
    return 1;
}

int bme_lab_json(char *body, size_t capacity)
{
    struct bme280_data data;
    if (bme280_get_sensor_data(BME280_ALL, &data, &sensor) != BME280_OK)
        return -1;

    lab_temperature = data.temperature;
    lab_pressure = data.pressure / 100.0;
    lab_humidity = data.humidity;
    return snprintf(body, capacity, "{\"status\":\"success\",\"temperature\":%.2f,"
                    "\"humidity\":%.2f,\"pressure\":%.2f}",
                    lab_temperature, lab_humidity, lab_pressure);
}

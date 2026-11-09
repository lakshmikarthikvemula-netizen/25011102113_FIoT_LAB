/* CAT 13 BME280 publisher using the supplied course ESPLib. */
#include "main.h"
#include "bme_lab.h"
#include "ESPLib.h"

extern UART_HandleTypeDef huart1;
#ifndef LAB_MQTT_TOPIC
#define LAB_MQTT_TOPIC "fiot/CHANGE_ME/bme280"
#endif
#ifndef LAB_MQTT_CLIENT
#define LAB_MQTT_CLIENT "fiot-CHANGE-ME"
#endif

void bme_mqtt_run(void)
{
    char json[160];
    MQTT_InitTypeDef_t settings = {0};

    if (!bme_lab_start()) {
        Error_Handler();
        return;
    }

    /* Supply local lab network and broker settings before enabling this application. */
    settings.keepAlive = 60;
    settings.connectFlag = CleanSession;
    settings.clientID = LAB_MQTT_CLIENT;
    if (ESP8266_Init(StationMode, &huart1) != funcOk ||
        ESP8266_wifiConnect("YOUR_SSID", "YOUR_PASSWORD") != funcOk ||
        ESP8266_portConnect("TCP", "YOUR_BROKER_HOST", "1883") != funcOk ||
        MQTT_connectBroker(&settings) != funcOk) {
        Error_Handler();
        return;
    }

    for (;;) {
        int length = bme_lab_json(json, sizeof(json));
        if (length > 0 && (size_t)length < sizeof(json) &&
            MQTT_publishTopic(LAB_MQTT_TOPIC, json, QoS0) != funcOk) {
            Error_Handler();
            return;
        }
        HAL_Delay(1100);
    }
}

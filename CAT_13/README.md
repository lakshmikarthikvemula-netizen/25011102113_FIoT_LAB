# CAT 13 — BME280 Sensor Interfacing Using I²C and (MQTT) using Custom Built ESP8266 API

[Evaluation handout](Assignment/CAT_13_Evaluation.pdf) · [CubeMX project](Source_Code/cat-13(1).ioc)

## Aim

Publish compensated BME280 measurements to an MQTT broker and display them in Node-RED.

## Components and software

NUCLEO-F401RE, BME280 breakout, ESP8266/Wemos, Bosch SensorAPI, course ESPLib, MQTT broker and Node-RED. Use the STM32 HAL and the toolchain selected when generating the CubeMX project.

## Circuit diagram

![CAT 13 circuit connections](Circuit_Diagram/wiring.svg)

For a Wemos D1 mini, use USB or the illustrated 5 V feed. Tie grounds together.
A bare ESP8266 module requires its own regulated 3.3 V supply and the boot-enable wiring for that module.
Use a 3.3 V BME280 interface and pull SCL/SDA up to 3.3 V if the breakout lacks pull-ups.
For a configurable breakout, hold CSB high for I²C and set SDO for address 0x76 or 0x77.

## Pin connections

| Component endpoint | STM32 pin or supply |
| --- | --- |
| RX · ESP8266/Wemos | PA9 · USART1 TX |
| TX · ESP8266/Wemos | PA10 · USART1 RX |
| GND · ESP8266/Wemos | GND |
| SCL · BME280 | PB6 · I²C1 SCL (CN10-17) |
| SDA · BME280 | PB7 · I²C1 SDA (CN7-21) |
| VDD · BME280 breakout | 3.3 V |
| GND · BME280 breakout | GND |
| Wemos 5 V input (if USB is disconnected) | External regulated 5 V supply |

## Working principle

The sensor adapter produces a JSON sample and the course MQTT API publishes it with QoS 0. The Node-RED function routes numeric measurements to three separate outputs.

## Procedure

Generate the STM32 project from the linked `.ioc` before adding application files. Add each C file to the build, place headers on the include path and call `bme_mqtt_run()` from the generated `main.c` after all peripheral initialization. Add the declaration `void bme_mqtt_run(void);` above `main()` in a CubeMX USER CODE section.

1. Use I²C1 and USART1 as configured in the uploaded project. The handout example uses USART2. The included adaptation uses the generated `huart1` handle. In CubeMX, enable **USART1 global interrupt** under **NVIC Settings** before generating code. Keep the USART1 baud rate matched to your ESP8266 AT firmware.
2. Add [the BME280 adapter](../Common/bme_lab.c), [its header](../Common/bme_lab.h) and [the Bosch driver](../Common/Bosch_BME280/). Enable float formatting in the project.
3. Add [ESPLib.c](Source_Code/ESPLib.c) to the build and put [ESPLib.h](Source_Code/ESPLib.h) on the include path. The library supplies `HAL_UART_RxCpltCallback()`. If your project already defines that callback, combine the handlers and route USART1 to the library. The CAT 13 publisher uses QoS 0. The handout’s `MQTTTimer()` hook applies to the library’s receive parser, which this publisher does not call.
4. Set Wi-Fi credentials, the TCP broker hostname, the topic and a unique client ID in your local copy of [bme_mqtt.c](Source_Code/bme_mqtt.c). This example uses a broker that accepts MQTT 3.1.1 on port 1883 without MQTT credentials. Keep the topic and JSON payload together at 123 bytes or less. Subscribe to that exact topic in Node-RED using MQTT In → JSON → Function. Paste [the three-output function](Source_Code/node_red_values.js) and wire outputs for temperature, humidity and pressure.

## Source code

[bme_mqtt.c](Source_Code/bme_mqtt.c). The generated application uses the supplied ESPLib by Can Güveren, adapted for the CubeMX USART1 handle, bounded serial I/O and packet sizes, and error reporting. The original handout and CubeMX configuration are included for reference.

## Expected output

Node-RED receives periodic MQTT messages with the three readings. The modem’s `SEND OK` indicates a successful TCP send. Use Node-RED to confirm broker delivery.

## Actual output and result

Physical execution and hardware output are pending.

- [ST Nucleo-64 board manual](https://www.st.com/resource/en/user_manual/dm00105823-nucleo-f401re-user-manual-stmicroelectronics.pdf)
- [Espressif ESP8266 Non-OS AT reference](https://www.espressif.com/sites/default/files/documentation/4a-esp8266_at_instruction_set_en.pdf)

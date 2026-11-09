# 25011102113_FIoT_LAB

Fundamentals of Internet of Things (FIoT) Laboratory — CAT assignments, circuit diagrams, source code, and documentation.

- **Register number** 25011102113
- **Institution** Shiv Nadar University Chennai
- **Course** Fundamentals of Internet of Things Laboratory

The supplied evaluations cover UART communication, GPIO, ESP8266 AT commands, HTTP, BME280 environmental sensing and MQTT. The CubeMX configurations target STM32F103C8 and NUCLEO-F401RE boards. The applications use Embedded C, STM32 HAL, CubeMX and Keil MDK. PuTTY or Tera Term, a browser and Node-RED are used for observation where applicable.

## Experiments

| CAT | Experiment | Documentation | Code |
| --- | --- | --- | --- |
| 09 | STM32 UART Command Controlled LED | [README](CAT_09/README.md) · [Handout](CAT_09/Assignment/CAT_09_Evaluation.pdf) | [Application](CAT_09/Source_Code/uart_led.c) |
| 10 | STM32–Wemos Wi-Fi Modem Interface Using AT Commands | [README](CAT_10/README.md) · [Handout](CAT_10/Assignment/CAT_10_Evaluation.pdf) | [Application](CAT_10/Source_Code/modem.c) |
| 10C | HTTP Server - STM32–Wemos Wi-Fi Modem Using AT Commands | [README](CAT_10C/README.md) · [Handout](CAT_10C/Assignment/CAT_10C_Evaluation.docx) | [Application](CAT_10C/Source_Code/http_lab.c) |
| 11 | STM32F401RE Cortex-M4: GPIO and User Input Programming | [README](CAT_11/README.md) · [Handout](CAT_11/Assignment/CAT_11_Evaluation.pdf) | [Application](CAT_11/Source_Code/gpio_lab.c) |
| 12 | BME280 Sensor Interfacing Using I²C and Bosch Sensor API | [README](CAT_12/README.md) · [Handout](CAT_12/Assignment/CAT_12_Evaluation.pdf) | [Application](CAT_12/Source_Code/bme_http.c) |
| 13 | BME280 Sensor Interfacing Using I²C and (MQTT) using Custom Built ESP8266 API | [README](CAT_13/README.md) · [Handout](CAT_13/Assignment/CAT_13_Evaluation.pdf) | [Application](CAT_13/Source_Code/bme_mqtt.c) |

## Repository layout

- Each CAT directory has a README with an embedded circuit diagram and pin table, an `Assignment/` directory with the original handout, and a `Source_Code/` directory with the original CubeMX settings and generated application examples.
- [Common](Common/) contains the shared ESP8266 AT and BME280 adapter code plus the Bosch driver. Follow each CAT's README when adding files to a CubeMX-generated project.
- [CAT 10C](CAT_10C/README.md) covers the HTTP server follow-up to [CAT 10](CAT_10/README.md). Its handout includes tasks 10C, 10D and 10E.
- The six supplied CubeMX metadata files are preserved alongside the corresponding MCU family and peripheral set. Files for CAT 9 and 10 matched byte for byte, as did the files for CAT 12 and 13.
- The generated examples require CubeMX project generation and bench validation. CAT 13 also requires the separately distributed course `ESPLib.c` and `ESPLib.h`. No physical execution evidence was supplied.

## Circuit diagrams

The CAT 12 diagram shows the BME280 and ESP8266 connections to the NUCLEO-F401RE. The experiment links above open the diagrams for the other configurations.

![CAT 12 BME280 and ESP8266 wiring](CAT_12/Circuit_Diagram/wiring.svg)

# Fundamentals of IoT Laboratory

**LAKSHMI KARTHIK VEMULA** · **25011102113**
Shiv Nadar University Chennai · Computer Science and Engineering

This repository follows the official Foundations-of-IoT evaluator layout: **29 projects across all 14 required CAT assignments (CAT2–CAT15)**. Existing correct work is preserved. Missing CAT14 and CAT15 projects use the user-designated Saketh reference with attribution.

The unchanged official evaluator reports **29/29 projects, 206/206 file checks, Overall PASSED** on the corrected checkout. Markdown and PDF reports are under [docs/evaluation](docs/evaluation/). This evaluates file structure and README availability; real compile/link checks and hardware status are documented separately.

The task projects contain **attributed reference-derived implementations and documented corrections**. Student identity in this README identifies the owner and does not claim sole authorship or independent physical completion. Read [source provenance, permissions and vendor licenses](docs/ATTRIBUTION.md) before academic submission or further redistribution.

## Validation

[Detailed validation](docs/VALIDATION.md) separates file existence, real compile/link results and physical verification. [Build instructions](docs/BUILDING.md) explain the tested SDCC/GNU ARM toolchains. Keil builds, CubeMX regeneration, simulator runs and hardware behavior are NOT VERIFIED. The supplied PDF's old PASS results are not reused as this repository's results.

## Task index

| Platform/CAT | Tasks |
|---|---|
| 8051/CAT2 | [TASK2B](8051/CAT2/TASK2B/README.md) |
| 8051/CAT3 | [TASK3A](8051/CAT3/TASK3A/README.md) |
| 8051/CAT4 | [TASK4A](8051/CAT4/TASK4A/README.md) |
| 8051/CAT5 | [TASK5A](8051/CAT5/TASK5A/README.md), [TASK5B.1](8051/CAT5/TASK5B/TASK5B.1/README.md), [TASK5B.2](8051/CAT5/TASK5B/TASK5B.2/README.md), [TASK5B.3](8051/CAT5/TASK5B/TASK5B.3/README.md), [TASK5C](8051/CAT5/TASK5C/README.md) |
| 8051/CAT6 | [TASK6A](8051/CAT6/TASK6A/README.md), [TASK6B](8051/CAT6/TASK6B/README.md) |
| 8051/CAT7 | [TASK7A](8051/CAT7/TASK7A/README.md), [TASK7B](8051/CAT7/TASK7B/README.md), [TASK7C](8051/CAT7/TASK7C/README.md) |
| STM32/CAT8 | [TASK8A](STM32/CAT8/TASK8A/README.md), [TASK8B](STM32/CAT8/TASK8B/README.md), [TASK8C](STM32/CAT8/TASK8C/README.md) |
| STM32/CAT9 | [TASK9A](STM32/CAT9/TASK9A/README.md), [TASK9B](STM32/CAT9/TASK9B/README.md) |
| STM32/CAT10 | [TASK10A](STM32/CAT10/TASK10A/README.md), [TASK10B](STM32/CAT10/TASK10B/README.md), [TASK10C](STM32/CAT10/TASK10C/README.md) |
| STM32/CAT11 | [TASK11A](STM32/CAT11/TASK11A/README.md), [TASK11B](STM32/CAT11/TASK11B/README.md) |
| STM32/CAT12 | [TASK12A](STM32/CAT12/TASK12A/README.md), [TASK12B](STM32/CAT12/TASK12B/README.md), [TASK12C](STM32/CAT12/TASK12C/README.md) |
| STM32/CAT13 | [TASK13](STM32/CAT13/TASK13/README.md) |
| STM32/CAT14 | [TASK14](STM32/CAT14/TASK14/README.md) |
| STM32/CAT15 | [TASK15](STM32/CAT15/TASK15/README.md), [Node-RED dashboard](STM32/CAT15/TASK15/Node-RED/README.md) |

## Layout

```text
8051/CAT2..CAT7/<TASK>/
  main.c
  STARTUP.A51
  <TASK>.uvproj
  Objects/firmware.hex
  README.md
  circuit.png              # PDF-compatible rendering; SVG source retained
STM32/CAT8..CAT15/<TASK>/
  <TASK>.ioc
  .mxproject
  Core/
  Drivers/
  MDK-ARM/<TASK>.uvprojx
  MDK-ARM/startup_*.s
  GNU/startup.c
  GNU/memory.ld
  Objects/firmware.hex
  README.md
  circuit.png              # PDF-compatible rendering; SVG source retained
docs/                   Provenance, mapping, build and validation records
tools/build.py          Reproducible real firmware builds
tools/validate.py       Report-pattern and integrity audit
```

CAT5B contains the required nested `TASK5B.1`, `TASK5B.2` and `TASK5B.3` folders. The report's single sensor task is `STM32/CAT13/TASK13`.

## Working with the repository

```sh
python3 tools/build.py
python3 tools/validate.py
sh tests/run_mqtt_tests.sh
```

Confirm board models and wiring against each task's schematic before flashing. GPIO mappings are derived from the configured source and have not been confirmed on a connected board. UART/ESP8266 examples need compatible 3.3 V signals, AT firmware and a suitable supply. Wi-Fi strings are placeholders; keep credentials out of Git.

See [migration and exclusions](docs/MIGRATION.md), [build evidence](docs/build_results.json), [validation evidence](docs/validation_results.json) and [progress](docs/PROGRESS.md).

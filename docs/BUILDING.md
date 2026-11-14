# Build instructions and validation limits

The automated build uses Python 3, SDCC 4.2.0 and GNU Arm Embedded GCC 12.2.1 with matching binutils and newlib 3.3.0. Install the appropriate packages for your OS. On Debian 12 these are `sdcc`, `sdcc-libraries`, `gcc-arm-none-eabi`, `binutils-arm-none-eabi`, `libnewlib-dev` and `libnewlib-arm-none-eabi`.

```sh
python3 tools/build.py                  # all 29 tasks
python3 tools/build.py 8051/CAT6        # both UART experiments
python3 tools/build.py STM32/CAT13      # sensor acquisition
python3 tools/validate.py
```

Compiler commands and diagnostics are saved in ignored `.build/<platform>/<CAT>/<task>/build.log`. Each successful task produces `Objects/firmware.hex`. `docs/build_results.json` records the source SHA-256, firmware SHA-256 and actual result. `docs/build_logs/` contains sanitized logs of the recorded build.

## Nonstandard toolchain installation

The build runs tools from PATH by default. Optional environment variables:

| Variable | Meaning |
|---|---|
| `SDCC` | SDCC executable |
| `SDCC_HOME` | SDCC shared-data directory containing `include/` and `lib/small/` |
| `ARM_GCC` | GNU Arm compiler executable |
| `ARM_OBJCOPY` | GNU Arm objcopy executable |
| `NEWLIB_INCLUDE` | newlib header directory, if not discovered automatically |
| `NEWLIB_LIB` | ARMv7-M soft-float newlib library directory, if not discovered automatically |

The recorded cloud builds used user-local extraction of official Debian packages, without system installation. Set these variables to your installation locations; the local cloud paths are not required by the repository.

## 8051

Application sources retain Keil C51 syntax. For SDCC, the script mechanically converts `sbit`, `sfr`, `bit`, `code`, `interrupt N` and register-header names in a temporary copy. Program logic is unchanged by that conversion. SDCC links against its own runtime/startup, with an 8KB code limit and 256-byte internal RAM limit for AT89S52. The resulting Intel HEX is freshly generated firmware, not copied from the reference.

The Keil project and `STARTUP.A51` are retained for the expected evaluator layout. Building in Keil C51 has **NOT VERIFIED** status; the SDCC result is not evidence that Keil itself passed. Software delays and manual timer reloads are approximate and require measurement at the actual oscillator frequency. CAT5B.2 specifically uses the AT89S52 T2MOD clock-out extension, not a generic 8051 feature.

## STM32

GNU builds use the C files and preprocessor defines listed in the Keil project. New GNU vector startup and linker scripts support the F103C8 (64KB flash/20KB RAM) and F401RE (512KB flash/96KB RAM). Builds use soft-float and newlib. GPIO, UART and I2C clocks/pins are explicitly initialized in source.

Keil MDK compilation and CubeMX regeneration have **NOT VERIFIED** status. CubeMX files are preserved and normalized, but source changes outside CubeMX user-code blocks can be overwritten by regeneration. Treat regeneration as a review-required development step: compare generated code with the recorded working source and rebuild before flashing. GNU linker success verifies symbol resolution and memory fit, not peripheral configuration or hardware operation.

## Physical validation still required

Confirm exact board revision, crystal/clock source, pin routing, LED and button polarity, port drive limits, reset circuit, ESP8266 firmware/baud rate and supply, BME280 address and I2C pull-ups. Test UART echo and overflow, modem responses, HTTP request handling, LCD/keypad movement, PWM/clock frequency and motor operation. Do not report these as observed results until they are actually measured.

Wi-Fi names/passwords are placeholders. Configure them locally without committing credentials. The repository retains its existing public visibility; no new private repository was created.

## MQTT regression tests

Run `sh tests/run_mqtt_tests.sh` with a host C compiler supporting AddressSanitizer and UndefinedBehaviorSanitizer. Both CAT14 and CAT15 transports are tested with deterministic mock UART responses. No device or network is required.

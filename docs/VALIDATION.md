# Validation — 14 November 2026

Scope: **29 projects, 14 required CAT assignments (CAT2–CAT15)**.

| Check | Baseline | Corrected checkout |
|---|---|---|
| Required CAT folders | 12/14; CAT14/CAT15 missing | 14/14 |
| Official discovered projects | 27/27 pass | 29/29 pass |
| Official file checks | 190/190 | 206/206 |
| Official overall | FAILED | PASSED |
| Official PDF generation | Crashed on circuit.svg | Generated successfully |
| Firmware builds in this session | Not executed before corrections | 29/29 compile/link PASS |
| MQTT host regression tests | Not present | PASS for both new projects, ASan/UBSan |
| Hardware, simulator, Keil, CubeMX | Unverified | Unverified |

Official evaluator source: https://github.com/Shrinidhi-Vyairavi-NP/Foundations-of-IoT-Evaluator at `8804159cef86ac7a578e611c6179b3859df0363f`. Neither the evaluator nor its tests were edited. The initial run used its CLI against the original public target. The local corrected-checkout run calls its original `generate_report` directly. A final CLI run against the pushed public repository records the final committed result.

Reports: [official evaluation files](evaluation/). The initial Markdown is preserved exactly. No initial PDF exists because the original evaluator failed while reading an SVG; the complete initial stderr/traceback is retained. No replacement initial PDF was invented.

Compilation evidence: [build_results.json](build_results.json), sanitized logs in [build_logs](build_logs/), and tracked `Objects/firmware.hex` in every task. The 13 8051 builds mechanically translate Keil syntax to SDCC; 16 STM32 builds use GNU Arm. These results do not establish Keil compatibility or runtime behavior.

Integrity evidence: [validation_results.json](validation_results.json). The local audit checks file patterns, Intel HEX record checksums, source/artifact hashes, Keil source references, image presence, local Markdown links, and scoped token/private-key patterns. Official file checks do not compile sources, evaluate README prose or prove circuit correctness.

MQTT tests: `sh tests/run_mqtt_tests.sh`. The mock UART checks exact CONNECT/PUBLISH bytes, broker rejection, two-byte Remaining Length, oversized input, invalid QoS/topic/AT arguments and finite timeout. No broker receives test traffic. Live broker delivery, physical wiring, electrical accuracy and assignment-handout compliance remain unverified.

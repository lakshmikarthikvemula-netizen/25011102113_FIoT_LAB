# Validation record

Recorded on 12 November 2026 for the requested report layout, excluding CAT14.

- Scope: 27 tasks, 12 CAT numbers (2–13).
- Report-pattern file checks: **190/190 PASS**.
- Fresh compile/link builds: **27/27 PASS** — 13 SDCC 8051 and 14 GNU ARM STM32 builds.
- Intel HEX checksums, main-source/artifact SHA-256, Keil source-file paths and local Markdown targets: **PASS**.
- Task README files and source-derived circuit SVG files: **27/27 present**.
- Keil C51/MDK, CubeMX regeneration, simulator execution and physical hardware: **NOT VERIFIED**.
- Official evaluator execution: **NOT VERIFIED**; only its PDF report was supplied.

The original PDF has 28 tasks/198 checks. CAT14 accounts for the excluded task and 8 checks; an unchanged official evaluator requiring CAT14 would still fail that requirement. Overall academic/hardware completion remains NOT VERIFIED.

## CAT summary

| CAT | Required Files | README | Circuit Diagram | Compilation | Hardware Verification | Overall Status |
|---|---|---|---|---|---|---|
| CAT2 (1 tasks) | PASS | PASS | PASS | PASS | NOT VERIFIED | NOT VERIFIED |
| CAT3 (1 tasks) | PASS | PASS | PASS | PASS | NOT VERIFIED | NOT VERIFIED |
| CAT4 (1 tasks) | PASS | PASS | PASS | PASS | NOT VERIFIED | NOT VERIFIED |
| CAT5 (5 tasks) | PASS | PASS | PASS | PASS | NOT VERIFIED | NOT VERIFIED |
| CAT6 (2 tasks) | PASS | PASS | PASS | PASS | NOT VERIFIED | NOT VERIFIED |
| CAT7 (3 tasks) | PASS | PASS | PASS | PASS | NOT VERIFIED | NOT VERIFIED |
| CAT8 (3 tasks) | PASS | PASS | PASS | PASS | NOT VERIFIED | NOT VERIFIED |
| CAT9 (2 tasks) | PASS | PASS | PASS | PASS | NOT VERIFIED | NOT VERIFIED |
| CAT10 (3 tasks) | PASS | PASS | PASS | PASS | NOT VERIFIED | NOT VERIFIED |
| CAT11 (2 tasks) | PASS | PASS | PASS | PASS | NOT VERIFIED | NOT VERIFIED |
| CAT12 (3 tasks) | PASS | PASS | PASS | PASS | NOT VERIFIED | NOT VERIFIED |
| CAT13 (1 tasks) | PASS | PASS | PASS | PASS | NOT VERIFIED | NOT VERIFIED |

Compilation PASS means a real complete firmware link using the toolchain documented in BUILDING.md. It does not mean the Keil project was executed or that the implementation behaves correctly on a board. Circuit PASS means the SVG exists and is linked; electrical/physical verification is pending.

## Evidence

- Machine-readable checks: [validation_results.json](validation_results.json).
- Builds and source/firmware hashes: [build_results.json](build_results.json).
- Sanitized compiler/linker transcripts: [build_logs](build_logs/).
- Scope and numbering: [MIGRATION.md](MIGRATION.md).
- Source permissions and attribution: [ATTRIBUTION.md](ATTRIBUTION.md).

## Remaining checks

Physical pin/board confirmation, measured timing, serial and RX overflow tests, modem AT-response verification, HTTP parsing/integration tests and real sensor readings remain unverified. The reference modem/HTTP examples are retained as attributed study implementations; the build result is not a claim of robust network behavior. Source-reference permission and instructor acceptance must be established before academic submission.

The credential scan is a scoped check for tokens, private keys and unrelated student IDs; it is not a guarantee that every possible secret format is detectable. Known reference Wi-Fi literals were replaced with placeholders. The existing repository is public and remains public.

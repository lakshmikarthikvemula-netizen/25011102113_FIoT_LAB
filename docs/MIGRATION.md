# Current scope and migration

The current request supersedes the checkout's previous CAT14 exclusion. The official evaluator requires 8051/CAT2–CAT7 and STM32/CAT8–CAT15. All 27 existing projects were preserved and two complete attributed projects were added at `STM32/CAT14/TASK14` and `STM32/CAT15/TASK15`, giving 29 projects and 206 file checks. The evaluator does not prescribe a fixed number of subtasks per CAT.

Baseline commit: `ed1a08429539d9cc621172e01fc2a3bec137ac37`. Backup branch: `backup/pre-evaluator-fixes-20261114`. Older work remains in Git history. No force push is used.

The baseline had all 190 discovered-project file checks passing but lacked CAT14/CAT15 and therefore failed overall. Its PDF generation crashed because ReportLab cannot decode the referenced SVG circuits. Each existing SVG now has a real PNG rendering and task README links point to PNG; SVG originals are retained. All previous firmware sources remain unchanged.

The donor CAT15 implementation was adapted for both missing CATs; its MQTT defects were corrected rather than importing nonworking CAT14 skeletons. Donor provenance is recorded in ATTRIBUTION.md. New complete projects contain actual Core/Drivers sources, CubeMX metadata, Keil configuration and startup, GNU build support, wiring diagrams and freshly compiled Intel HEX files.

No handouts exist in the current checkout, so assignment-specific equivalence beyond available source and reference documentation remains unverified. No hardware evidence is fabricated.

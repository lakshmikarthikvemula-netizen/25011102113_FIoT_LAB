# Source provenance and third-party material

Student documentation belongs to LAKSHMI KARTHIK VEMULA, registration 25011102113. This identifies the repository owner; it does not transfer authorship of reference-derived implementations.

## Existing work

Previous repository work and its handouts remain recoverable from Git history, including commit `901fc50b14ff2f9510b39a7a4db177333fa58b5c`. The backup folder was removed at the owner’s request. Required Bosch code and its license are included directly in the active sensor project. No new third-party handouts were imported.

## Reference-derived task projects

At the owner's explicit request, the report-layout task projects were adapted from:

- Source: https://github.com/thanishq-bvrm/25011102018_FIoT_LAB
- Source commit: `a3af69988a16451a682e46a2dae72dcd1023d181`
- Scope: the 27 pre-existing task paths through CAT13. The report's `STM32/CAT13/TASK13` is adapted from reference `STM32/CAT13/TASK13A`.

The source repository has no root license granting general reuse of student-authored application code or documentation. Public availability is not a license. Importing it at the owner's request does not establish the reference author's permission or an academic right to submit it. These projects are identified as reference-derived study/adaptation material; obtain the necessary author/instructor permission before submitting them as laboratory work or redistributing them further. No blanket license is asserted over these materials.

Reference README prose, copied HEX files and circuit images were not retained in the evaluated task folders. New documentation distinguishes expected behavior, successful builds and untested physical results. Source comments/copyright notices were preserved, except credential literals were replaced by placeholders.

## Corrections and additions

- Replaced the reference's polling UART echo with an RX-interrupt ring buffer.
- Replaced delay-loop PWM and stepper timing with hardware-timer-driven variants.
- Used AT89S52 Timer2 hardware clock output for variable-frequency pulse generation.
- Changed LCD/keypad interaction to bounded directional character movement.
- Fixed a missing C brace in the NUCLEO long-press example.
- Added STM32 UART/I2C peripheral clock and alternate-function setup absent from the imported MSP skeletons.
- Kept SWD debugging enabled on the Blue Pill.
- Corrected NUCLEO MB1136 button polarity and stable-state debounce.
- Replaced synthetic BME280 values with the existing repository's Bosch adapter and compensated sensor driver.
- Added GNU startup/linker files, reproducible builds, a validator and software-derived SVG pin-connection diagrams.

These changes do not prove compliance with assignment requirements that were not supplied or verified on hardware.

## Vendor and library licenses

- ST HAL/device files retain their original copyright and adjacent `LICENSE.txt` files (BSD-3-Clause and/or applicable original notices).
- ARM CMSIS files retain their included license/notices. STM32F4 CMSIS includes `Drivers/CMSIS/LICENSE.txt`; STM32F1 files carry their own source notices, with device/HAL license files preserved.
- Keil `STARTUP.A51` retains Keil copyright and package notices. It is reference-provided material, not newly authored startup code; use it under the applicable Keil terms. The tested SDCC build uses SDCC's startup instead.
- Bosch BME280 SensorAPI v3.5.1 retains its BSD-3-Clause license in `STM32/CAT13/TASK13/Core/BOSCH_LICENSE`.

Personal student details and network credentials from the reference were not used as this student's details. Reference identification appears here solely to document provenance.

## CAT14 and CAT15 additions (14 November 2026)

At the user's explicit request, added material from https://github.com/saketh3159/25011102092_FIoT_LAB at commit `1eb9b35fc7c64b5af9bed676f3a492881984d5ef`.

Both new projects derive from `STM32/CAT15/TASK15`. The donor CAT14 skeleton only checked sensor presence and sent ping bytes, so its unverified telemetry claims and incomplete subtask implementations were not copied. CAT14 now adapts the complete sensor project with PB8/PB9 I2C wiring. CAT15 uses PB6/PB7. Replaced unsafe polling/interrupt buffer handling with a bounded polling MQTT transport, checked CONNACK, variable Remaining Length encoding, input bounds, finite timeouts, and no stale-sample publishing. Added GNU startup/linker files from this repository's existing F401 project and retained Bosch license notices. New pin diagrams, documentation, host regression tests and the demo-labeled Node-RED dashboard were created for this repository. No donor hardware-success claims or screenshots are presented as this owner's measurements.

ESPLib's original author is Can Guveren (2022); the small public interface is adapted, while the new transport implementation is documented in source. Existing ST/ARM/Bosch notices are retained. No new blanket license or sole-authorship claim is made for donor-derived material.

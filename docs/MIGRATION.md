# Migration and scope

The owner authorized modifying the existing `25011102113_FIoT_LAB` repository, confirmed the registration number (without the extra zero), selected the report's platform/CAT/task layout and explicitly excluded CAT14. The existing public visibility is unchanged. Git history preserves earlier versions. The backup folder was removed at the owner’s request; required active-project dependencies remain in the platform folders.

The supplied report discovered 28 tasks and performed 198 file checks. With CAT14 explicitly excluded, the active manifest has **27 tasks, 12 distinct CAT numbers (2–13), and 190 file checks**. This is the requested scope adjustment, not a claim that the original 28-task evaluator will pass unchanged. An evaluator that still requires CAT14 will report it missing.

| Previous folder | Corresponding subject in report layout |
|---|---|
| CAT_02 | 8051/CAT2 |
| CAT_03 | 8051/CAT3 |
| CAT_04 and CAT_08 | 8051/CAT4 keypad/display topics; original files available in Git history |
| CAT_05 | 8051/CAT5 |
| CAT_06 | 8051/CAT6 |
| CAT_07 | 8051/CAT7 |
| CAT_09 | STM32/CAT9 UART topics |
| CAT_10 | STM32/CAT10 modem topics |
| CAT_10C | STM32/CAT11 HTTP topics |
| CAT_11 | STM32/CAT12 NUCLEO GPIO topics |
| CAT_12 | STM32/CAT13 sensor topics; original HTTP/Node-RED code available in Git history |
| CAT_13 | Original MQTT work available in Git history; no CAT14 added |

Report paths identify structure; they do not establish equivalence of all academic tasks. The task READMEs follow actual implementation behavior, because some reference README titles conflicted with application sources. Reference projects remain attributed and their license/academic permission status is documented in ATTRIBUTION.md.

No official evaluator executable was supplied. The repository validator reproduces the report's documented file patterns, validates Intel HEX checksums and source/artifact hashes, checks project-file references and local Markdown links, and writes its own validation record. It is not presented as the official evaluator.

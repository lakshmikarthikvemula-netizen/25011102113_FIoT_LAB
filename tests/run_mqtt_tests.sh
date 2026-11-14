#!/bin/sh
set -eu
cd "$(dirname "$0")/.."
mkdir -p .build/tests
for task in STM32/CAT14/TASK14 STM32/CAT15/TASK15; do
  cc -std=c11 -Wall -Wextra -Werror -fsanitize=address,undefined -Itests -I"$task/Core/Inc" tests/test_mqtt.c "$task/Core/Src/ESPLib.c" -o .build/tests/test_mqtt
  .build/tests/test_mqtt
done

#!/usr/bin/env bash

tests=(
  "8 9 *|72|0"
  "8 9 +|17|0"
  "9 3 -|6|0"
  "8 2 /|4|0"
  "7 7 + 3 /|4|0"
  "5 1 2 + 4 * + 3 -|14|0"
  "8 9 * 9 - 9 - 9 - 4 - 1 +|42|0"
  "1 2 + 3 + 4 +|10|0"
  "9 1 - 2 /|4|0"
  "2 3 * 4 * 5 *|120|0"
  "9 9 * 9 * 9 *|6561|0"
  "3 4 + 2 * 7 /|2|0"
  "0 0 +|0|0"
  "9 2 /|4|0"
  "1|1|0"
  "+|Error|1"
  "1 +|Error|1"
  "1 2 + +|Error|1"
  "1 2 3 + + +|Error|1"
  "1 2 3 +|Error|1"
  "1 2|Error|1"
  "12 3 +|Error|1"
  "89*|Error|1"
  "1 2+ |Error|1"
  "a b +|Error|1"
  "2 3 &|Error|1"
  "2 -3 +|Error|1"
  "4 0 /|Error|1"
  "0 0 /|Error|1"
)

passed=0
failed=0
binary="./RPN"

if [ ! -x "$binary" ]; then
  echo "No se ha encontrado ejecutable: $binary"
  exit 1
fi

index=1
for test in "${tests[@]}"; do
  IFS='|' read -r input expected expected_exit <<< "$test"

  output=$("$binary" "$input" 2>&1)
  exit_code=$?

  if [ "$output" = "$expected" ] && [ "$exit_code" -eq "$expected_exit" ]; then
    passed=$((passed + 1))
    printf '[%d] OK    input: %s\n' "$index" "$input"
  else
    failed=$((failed + 1))
    printf '[%d] FAIL  input: %s\n' "$index" "$input"
    printf '     esperado: %s\n' "$expected"
    printf '     obtenido: %s\n' "$output"
    printf '     exit code: %s\n' "$exit_code"
  fi
  index=$((index + 1))
done

echo
printf 'Total: %d\n' "${#tests[@]}"
printf 'OK:    %d\n' "$passed"
printf 'FAIL:  %d\n' "$failed"

if [ "$failed" -ne 0 ]; then
  exit 1
fi
exit 0

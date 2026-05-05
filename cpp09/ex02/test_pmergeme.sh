#!/usr/bin/env bash

binary="./PmergeMe"
passed=0
failed=0

run_ok_test() {
  local input="$1"
  local output
  local before
  local after
  local expected

  output=$($binary $input 2>&1)
  if [ $? -ne 0 ]; then
    echo "[FAIL] $input"
    echo "  unexpected non-zero exit"
    echo "  output: $output"
    failed=$((failed + 1))
    return
  fi

  before=$(printf '%s\n' "$output" | sed -n 's/^Before: //p' | head -n 1)
  after=$(printf '%s\n' "$output" | sed -n 's/^After:  //p' | head -n 1)
  expected=$(printf '%s\n' $input | tr ' ' '\n' | sort -n | tr '\n' ' ' | sed 's/ $//')

  if [ "$before" != "$input" ]; then
    echo "[FAIL] $input"
    echo "  Before incorrect: $before"
    failed=$((failed + 1))
    return
  fi
  if [ "$after" != "$expected" ]; then
    echo "[FAIL] $input"
    echo "  expected After: $expected"
    echo "  got After:      $after"
    failed=$((failed + 1))
    return
  fi
  echo "[OK]   $input"
  passed=$((passed + 1))
}

run_error_test() {
  local input="$1"
  local output

  output=$($binary $input 2>&1)
  if [ $? -eq 0 ] || [ "$output" != "Error" ]; then
    echo "[FAIL] $input"
    echo "  expected Error with non-zero exit"
    echo "  output: $output"
    failed=$((failed + 1))
    return
  fi
  echo "[OK]   error case: $input"
  passed=$((passed + 1))
}

if [ ! -x "$binary" ]; then
  echo "No se ha encontrado ejecutable: $binary"
  exit 1
fi

run_ok_test "3 2 1"
run_ok_test "4 9 1 7 3"
run_ok_test "8 3 5 2 9 7"
run_ok_test "10 42 5 17 8 99 1"
run_ok_test "11 3 8 6 2 9 5 1"
run_ok_test "5 5 4 4 3 3 2 2 1 1"
run_ok_test "42"
run_ok_test "1000 2 999 3 998 4 997 5"
run_ok_test "7 12 1 19 8 3 15 10 6 4"
run_ok_test "9 8 7 6 5 4 3 2 1"

run_error_test "0 1 2"
run_error_test "-1 2 3"
run_error_test "3 a 1"
run_error_test "2147483648 1"

echo
echo "Total: $((passed + failed))"
echo "OK:    $passed"
echo "FAIL:  $failed"

if [ $failed -ne 0 ]; then
  exit 1
fi
exit 0

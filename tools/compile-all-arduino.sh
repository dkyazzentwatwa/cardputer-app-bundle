#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_ROOT="${ROOT}/_arduino-build"

mkdir -p "${BUILD_ROOT}"
FAILURES=()

compile_app() {
  local slug="$1"
  local profile="$2"
  local app_dir="${ROOT}/${slug}"
  local build_dir="${BUILD_ROOT}/${slug}"
  shift 2

  if [[ ! -d "${app_dir}" ]]; then
    echo "[arduino] missing app directory: ${app_dir}" >&2
    return 1
  fi

  if [[ ! -f "${app_dir}/${slug}.ino" ]]; then
    echo "[arduino] missing sketch entrypoint: ${app_dir}/${slug}.ino" >&2
    return 1
  fi

  if [[ ! -f "${app_dir}/sketch.yaml" ]]; then
    echo "[arduino] missing sketch profile: ${app_dir}/sketch.yaml" >&2
    return 1
  fi

  rm -rf "${build_dir}"
  mkdir -p "${build_dir}"

  local args=(
    compile
    --profile "${profile}"
    --build-path "${build_dir}"
    --warnings none
  )

  args+=("$@" "${app_dir}")

  echo
  echo "[arduino] building ${slug} (${profile})"
  printf '[arduino] command: arduino-cli'
  printf ' %q' "${args[@]}"
  printf '\n'
  if ! arduino-cli "${args[@]}"; then
    FAILURES+=("${slug}")
    return 0
  fi
}

compile_app "Bitcoin-Card-Wallet" "cardputer" --build-property "compiler.c.elf.extra_flags=-Wl,-zmuldefs"
compile_app "Cardputer-Game-Station-Emulators" "cardputer" --build-property "compiler.c.elf.extra_flags=-Wl,-zmuldefs -Wl,--wrap=bmp_create -Wl,--wrap=bmp_destroy"
compile_app "ESP32-Bit-Pirate" "cardputer-adv" --build-property "compiler.c.elf.extra_flags=-Wl,-zmuldefs"
compile_app "Esp32-USB-Stick" "cardputer"
compile_app "News-Reader" "cardputer"
compile_app "Open-Wifi-Scanner" "cardputer"
compile_app "Password-Manager" "cardputer"
compile_app "Ultimate-Remote" "cardputer-adv"

echo
if [[ "${#FAILURES[@]}" -ne 0 ]]; then
  echo "[arduino] failed apps: ${FAILURES[*]}" >&2
  exit 1
fi

echo "[arduino] all staged Cardputer apps compiled"

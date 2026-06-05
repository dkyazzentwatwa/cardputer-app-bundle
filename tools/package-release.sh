#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_ROOT="${ROOT}/_arduino-build"
RELEASE_ROOT="${ROOT}/dist/release"
REPORT="${RELEASE_ROOT}/BUILD_REPORT.md"

APP_BINS=(
  "Bitcoin-Card-Wallet:bitcoin-card-wallet.bin"
  "Cardputer-Game-Station-Emulators:cardputer-game-station-emulators.bin"
  "ESP32-Bit-Pirate:esp32-bit-pirate.bin"
  "Esp32-USB-Stick:esp32-usb-stick.bin"
  "News-Reader:news-reader.bin"
  "Open-Wifi-Scanner:open-wifi-scanner.bin"
  "Password-Manager:password-manager.bin"
  "Ultimate-Remote:ultimate-remote.bin"
)

rm -rf "${RELEASE_ROOT}"
mkdir -p "${RELEASE_ROOT}"

"${ROOT}/tools/compile-all-arduino.sh"

{
  echo "# Cardputer App Bundle Build Report"
  echo
  echo "- Generated: \`$(date -u +%Y-%m-%dT%H:%M:%SZ)\`"
  echo "- Source: \`https://github.com/dkyazzentwatwa/cardputer-app-bundle\`"
  echo
  echo "| App | Binary | Size | SHA256 |"
  echo "| --- | --- | ---: | --- |"
} > "${REPORT}"

for item in "${APP_BINS[@]}"; do
  app="${item%%:*}"
  dest="${item#*:}"
  src="$(find "${BUILD_ROOT}/${app}" -maxdepth 1 -type f -name '*.ino.bin' | head -n 1 || true)"
  if [[ -z "${src}" ]]; then
    echo "[release] missing sketch app bin for ${app}" >&2
    exit 1
  fi
  cp -f "${src}" "${RELEASE_ROOT}/${dest}"
  size="$(wc -c < "${RELEASE_ROOT}/${dest}" | tr -d ' ')"
  sha="$(shasum -a 256 "${RELEASE_ROOT}/${dest}" | awk '{print $1}')"
  echo "| ${app} | \`${dest}\` | ${size} | \`${sha}\` |" >> "${REPORT}"
done

echo "[release] prepared ${RELEASE_ROOT}"

# Cardputer App Bundle

A beginner-friendly staging collection of experimental apps for the
[M5Stack Cardputer](https://shop.m5stack.com/products/m5stack-cardputer-kit-w-m5stamps3).

This repo gathers several Cardputer projects in one place and adapts them for
local Arduino CLI builds. It is the public source bundle used by Cypher OS for
these catalog apps, while still being useful for learning, tinkering, and
testing individual app ideas.

If you want the integrated Cardputer OS that brings these apps together, see
[dkyazzentwatwa/cypher-puter-os](https://github.com/dkyazzentwatwa/cypher-puter-os).

## What Is In Here

Each folder is its own app:

| App | What it does | Start here |
| --- | --- | --- |
| `Bitcoin-Card-Wallet` | Offline Bitcoin wallet and signing experiments. | `Bitcoin-Card-Wallet/README.md` |
| `Cardputer-Game-Station-Emulators` | Retro console emulator launcher for ROMs on SD card. | `Cardputer-Game-Station-Emulators/README.md` |
| `ESP32-Bit-Pirate` | Multi-protocol hardware and serial exploration tool. | `ESP32-Bit-Pirate/README.md` |
| `Esp32-USB-Stick` | Exposes the Cardputer SD card as USB mass storage. | `Esp32-USB-Stick/README.md` |
| `News-Reader` | Simple Wi-Fi news search reader using The Guardian API. | `News-Reader/README.md` |
| `Open-Wifi-Scanner` | Scans nearby Wi-Fi networks and labels open/weak networks. | `Open-Wifi-Scanner/README.md` |
| `Password-Manager` | Encrypted password vault with USB keyboard typing. | `Password-Manager/README.md` |
| `Ultimate-Remote` | Infrared universal remote with a large command database. | `Ultimate-Remote/README.md` |

Most apps started as separate upstream projects and were staged here with local
Cardputer build files, splash/theme work, and Arduino CLI profiles.

## Hardware You Need

For the easiest path, use:

- M5Stack Cardputer or Cardputer ADV, depending on the app profile.
- USB-C cable that supports data, not only charging.
- microSD card for apps that store files, ROMs, vaults, or logs.
- A computer with Arduino CLI installed.

Some apps need extra hardware or services:

- `Bitcoin-Card-Wallet` can use an RFID2 module and MIFARE tags.
- `News-Reader` needs Wi-Fi and a Guardian API key.
- `Ultimate-Remote` uses the Cardputer IR transmitter.
- `ESP32-Bit-Pirate` may use external modules depending on the protocol.

## Install Arduino CLI

Install Arduino CLI from:

https://arduino.github.io/arduino-cli/latest/installation/

Then confirm it works:

```sh
arduino-cli version
```

The app `sketch.yaml` files list the M5Stack board package and library
dependencies. Arduino CLI can use those profiles directly.

## Build One App

From the repo root, compile one app like this:

```sh
arduino-cli compile --profile cardputer News-Reader
```

Most apps use the `cardputer` profile. A few also have a `cardputer-adv`
profile for Cardputer ADV builds.

Examples:

```sh
arduino-cli compile --profile cardputer Password-Manager
arduino-cli compile --profile cardputer Open-Wifi-Scanner
arduino-cli compile --profile cardputer-adv ESP32-Bit-Pirate
arduino-cli compile --profile cardputer-adv Ultimate-Remote
```

## Build Everything

This repo includes a helper script that compiles every staged app into the local
`_arduino-build/` folder:

```sh
./tools/compile-all-arduino.sh
```

`_arduino-build/` is ignored by Git because it contains generated build output.

## Package Release Binaries

Build all apps and collect public `.bin` assets under `dist/release/`:

```sh
./tools/package-release.sh
```

The release package contains sketch app binaries only, not merged images,
bootloader images, or partition-table images.

## Flashing A Cardputer

First plug in your Cardputer and find its port:

```sh
arduino-cli board list
```

Then compile and upload one app:

```sh
arduino-cli compile --profile cardputer --upload --port /dev/cu.usbmodemXXXX News-Reader
```

Replace `/dev/cu.usbmodemXXXX` with the port shown on your machine.

Do not flash random apps to important hardware without reading that app's README
first. Some apps store data on SD cards, emulate USB devices, transmit infrared,
or interact with nearby networks and hardware pins.

## App Setup Notes

`News-Reader` needs a local secrets file:

```sh
cp News-Reader/src/secrets.h.template News-Reader/src/secrets.h
```

Then edit `News-Reader/src/secrets.h` with your Wi-Fi name, Wi-Fi password, and
Guardian API key. That file is ignored by Git.

For Cypher OS SD-card builds, prefer `/news-reader/config.txt` on the SD card
instead of compiling local secrets into the app binary.

For emulator apps, bring your own legal ROM files. This repo does not include
game ROMs.

For password and wallet apps, treat the Cardputer as experimental hardware.
Back up anything important outside the device, and do not store money or
critical passwords here until you fully understand the code and risks.

## Safety And Legal Notes

These apps are for learning and personal experimentation.

- Only scan networks you own or have permission to test.
- Only use protocol tools on hardware you own or have permission to inspect.
- Do not use IR, radio, Wi-Fi, RFID, or USB features to disrupt other people.
- Do not rely on experimental firmware as your only wallet or password backup.

If you are new to this kind of hardware work, start with `News-Reader`,
`Esp32-USB-Stick`, or `Open-Wifi-Scanner` before trying the more advanced tools.

## Repo Layout

```text
.
|-- Bitcoin-Card-Wallet/
|-- Cardputer-Game-Station-Emulators/
|-- ESP32-Bit-Pirate/
|-- Esp32-USB-Stick/
|-- News-Reader/
|-- Open-Wifi-Scanner/
|-- Password-Manager/
|-- Ultimate-Remote/
`-- tools/
```

Every app folder has a root `.ino` file and a `sketch.yaml` build profile.

## Contributing

Small fixes are welcome:

- README improvements
- compile fixes
- beginner setup notes
- safer defaults
- app-specific bug reports

Please keep generated files out of commits. Build outputs, local secrets,
`.DS_Store`, and local nested Git backups are intentionally ignored.

## Status

This is a source bundle for experimental Cardputer apps. Apps may compile,
flash, or behave differently depending on your board revision, SD card,
installed Arduino packages, and attached modules.

Build success means the source compiled. It does not automatically prove the app
has been tested on your exact hardware.

For the more complete integrated experience, use
[cypher-puter-os](https://github.com/dkyazzentwatwa/cypher-puter-os). This repo
is useful when you want to inspect, build, or adapt the individual app sources.

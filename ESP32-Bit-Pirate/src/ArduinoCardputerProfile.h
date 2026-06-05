#pragma once

#ifndef ESP32_BIT_PIRATE_ARDUINO_CARDPUTER_PROFILE_H
#define ESP32_BIT_PIRATE_ARDUINO_CARDPUTER_PROFILE_H

#ifndef DEVICE_CARDPUTER
#define DEVICE_CARDPUTER 1
#endif

#if defined(BOARD_HAS_PSRAM) && !defined(DEVICE_CARDPUTERADV)
#define DEVICE_CARDPUTERADV 1
#endif

#ifndef LED_PIN
#define LED_PIN 21
#endif

#ifndef LED_TYPE_RGB
#define LED_TYPE_RGB true
#endif

#ifndef UART_BAUD
#define UART_BAUD 9600
#endif

#ifndef I2C_FREQ
#define I2C_FREQ 100000
#endif

#ifndef I2S_BCLK_PIN
#define I2S_BCLK_PIN 41
#endif

#ifndef I2S_LRCK_PIN
#define I2S_LRCK_PIN 43
#endif

#ifndef I2S_DATA_PIN
#define I2S_DATA_PIN 42
#endif

#ifndef I2S_SAMPLE_RATE
#define I2S_SAMPLE_RATE 44100
#endif

#ifndef I2S_BITS
#define I2S_BITS 16
#endif

#ifndef CAN_KBPS
#define CAN_KBPS 125
#endif

#ifndef SDCARD_CS_PIN
#define SDCARD_CS_PIN 12
#endif

#ifndef SDCARD_CLK_PIN
#define SDCARD_CLK_PIN 40
#endif

#ifndef SDCARD_MISO_PIN
#define SDCARD_MISO_PIN 39
#endif

#ifndef SDCARD_MOSI_PIN
#define SDCARD_MOSI_PIN 14
#endif

#ifndef IR_TX_PIN
#define IR_TX_PIN 44
#endif

#ifndef IR_RX_PIN
#define IR_RX_PIN 1
#endif

#if defined(DEVICE_CARDPUTERADV)
#ifndef PROTECTED_PINS
#define PROTECTED_PINS "19,20,22,23,24,25,27,28,29,30,31,32,33,34,35,36,37,38"
#endif
#ifndef ONEWIRE_PIN
#define ONEWIRE_PIN 3
#endif
#ifndef TWOWIRE_CLK_PIN
#define TWOWIRE_CLK_PIN 1
#endif
#ifndef TWOWIRE_IO_PIN
#define TWOWIRE_IO_PIN 2
#endif
#ifndef TWOWIRE_RST_PIN
#define TWOWIRE_RST_PIN 3
#endif
#ifndef THREEWIRE_CS_PIN
#define THREEWIRE_CS_PIN 1
#endif
#ifndef THREEWIRE_SK_PIN
#define THREEWIRE_SK_PIN 2
#endif
#ifndef THREEWIRE_DI_PIN
#define THREEWIRE_DI_PIN 3
#endif
#ifndef THREEWIRE_DO_PIN
#define THREEWIRE_DO_PIN 4
#endif
#ifndef UART_RX_PIN
#define UART_RX_PIN 1
#endif
#ifndef UART_TX_PIN
#define UART_TX_PIN 2
#endif
#ifndef HDUART_BAUD
#define HDUART_BAUD 9600
#endif
#ifndef HDUART_PIN
#define HDUART_PIN 3
#endif
#ifndef I2C_SCL_PIN
#define I2C_SCL_PIN 1
#endif
#ifndef I2C_SDA_PIN
#define I2C_SDA_PIN 2
#endif
#ifndef SPI_CS_PIN
#define SPI_CS_PIN 5
#endif
#ifndef SPI_CLK_PIN
#define SPI_CLK_PIN 15
#endif
#ifndef SPI_MISO_PIN
#define SPI_MISO_PIN 13
#endif
#ifndef SPI_MOSI_PIN
#define SPI_MOSI_PIN 39
#endif
#ifndef LED_DATA_PIN
#define LED_DATA_PIN 5
#endif
#ifndef LED_CLOCK_PIN
#define LED_CLOCK_PIN 15
#endif
#ifndef CAN_CS_PIN
#define CAN_CS_PIN 5
#endif
#ifndef CAN_SCK_PIN
#define CAN_SCK_PIN 15
#endif
#ifndef CAN_SO_PIN
#define CAN_SO_PIN 13
#endif
#ifndef CAN_SI_PIN
#define CAN_SI_PIN 39
#endif
#ifndef ETHERNET_CS_PIN
#define ETHERNET_CS_PIN 5
#endif
#ifndef ETHERNET_CLK_PIN
#define ETHERNET_CLK_PIN 15
#endif
#ifndef ETHERNET_MISO_PIN
#define ETHERNET_MISO_PIN 13
#endif
#ifndef ETHERNET_MOSI_PIN
#define ETHERNET_MOSI_PIN 39
#endif
#ifndef ETHERNET_IRQ_PIN
#define ETHERNET_IRQ_PIN 3
#endif
#ifndef SUBGHZ_CS_PIN
#define SUBGHZ_CS_PIN 5
#endif
#ifndef SUBGHZ_SCK_PIN
#define SUBGHZ_SCK_PIN 15
#endif
#ifndef SUBGHZ_SI_PIN
#define SUBGHZ_SI_PIN 13
#endif
#ifndef SUBGHZ_SO_PIN
#define SUBGHZ_SO_PIN 39
#endif
#ifndef SUBGHZ_GDO_PIN
#define SUBGHZ_GDO_PIN 3
#endif
#ifndef RF24_CSN_PIN
#define RF24_CSN_PIN 5
#endif
#ifndef RF24_CE_PIN
#define RF24_CE_PIN 3
#endif
#ifndef RF24_SCK_PIN
#define RF24_SCK_PIN 15
#endif
#ifndef RF24_MISO_PIN
#define RF24_MISO_PIN 39
#endif
#ifndef RF24_MOSI_PIN
#define RF24_MOSI_PIN 13
#endif
#ifndef JTAG_SCAN_PINS
#define JTAG_SCAN_PINS "1, 2, 13, 39, 5, 15, 40"
#endif
#else
#ifndef PROTECTED_PINS
#define PROTECTED_PINS "8,9,11,19,20,22,23,24,25,27,28,29,30,31,32,33,34,35,36,37,38"
#endif
#ifndef ONEWIRE_PIN
#define ONEWIRE_PIN 1
#endif
#ifndef TWOWIRE_CLK_PIN
#define TWOWIRE_CLK_PIN 1
#endif
#ifndef TWOWIRE_IO_PIN
#define TWOWIRE_IO_PIN 2
#endif
#ifndef TWOWIRE_RST_PIN
#define TWOWIRE_RST_PIN 12
#endif
#ifndef THREEWIRE_CS_PIN
#define THREEWIRE_CS_PIN 12
#endif
#ifndef THREEWIRE_SK_PIN
#define THREEWIRE_SK_PIN 40
#endif
#ifndef THREEWIRE_DI_PIN
#define THREEWIRE_DI_PIN 39
#endif
#ifndef THREEWIRE_DO_PIN
#define THREEWIRE_DO_PIN 14
#endif
#ifndef UART_RX_PIN
#define UART_RX_PIN 1
#endif
#ifndef UART_TX_PIN
#define UART_TX_PIN 2
#endif
#ifndef HDUART_BAUD
#define HDUART_BAUD 9600
#endif
#ifndef HDUART_PIN
#define HDUART_PIN 1
#endif
#ifndef I2C_SCL_PIN
#define I2C_SCL_PIN 1
#endif
#ifndef I2C_SDA_PIN
#define I2C_SDA_PIN 2
#endif
#ifndef SPI_CS_PIN
#define SPI_CS_PIN 12
#endif
#ifndef SPI_CLK_PIN
#define SPI_CLK_PIN 40
#endif
#ifndef SPI_MISO_PIN
#define SPI_MISO_PIN 39
#endif
#ifndef SPI_MOSI_PIN
#define SPI_MOSI_PIN 14
#endif
#ifndef LED_DATA_PIN
#define LED_DATA_PIN 1
#endif
#ifndef LED_CLOCK_PIN
#define LED_CLOCK_PIN 2
#endif
#ifndef CAN_CS_PIN
#define CAN_CS_PIN 12
#endif
#ifndef CAN_SCK_PIN
#define CAN_SCK_PIN 40
#endif
#ifndef CAN_SI_PIN
#define CAN_SI_PIN 39
#endif
#ifndef CAN_SO_PIN
#define CAN_SO_PIN 14
#endif
#ifndef ETHERNET_CS_PIN
#define ETHERNET_CS_PIN 12
#endif
#ifndef ETHERNET_CLK_PIN
#define ETHERNET_CLK_PIN 40
#endif
#ifndef ETHERNET_MISO_PIN
#define ETHERNET_MISO_PIN 39
#endif
#ifndef ETHERNET_MOSI_PIN
#define ETHERNET_MOSI_PIN 14
#endif
#ifndef ETHERNET_IRQ_PIN
#define ETHERNET_IRQ_PIN 1
#endif
#ifndef SUBGHZ_CS_PIN
#define SUBGHZ_CS_PIN 12
#endif
#ifndef SUBGHZ_SCK_PIN
#define SUBGHZ_SCK_PIN 40
#endif
#ifndef SUBGHZ_SI_PIN
#define SUBGHZ_SI_PIN 39
#endif
#ifndef SUBGHZ_SO_PIN
#define SUBGHZ_SO_PIN 14
#endif
#ifndef SUBGHZ_GDO_PIN
#define SUBGHZ_GDO_PIN 1
#endif
#ifndef RF24_CSN_PIN
#define RF24_CSN_PIN 12
#endif
#ifndef RF24_CE_PIN
#define RF24_CE_PIN 1
#endif
#ifndef RF24_SCK_PIN
#define RF24_SCK_PIN 40
#endif
#ifndef RF24_MISO_PIN
#define RF24_MISO_PIN 14
#endif
#ifndef RF24_MOSI_PIN
#define RF24_MOSI_PIN 39
#endif
#ifndef JTAG_SCAN_PINS
#define JTAG_SCAN_PINS "1, 2, 12, 40"
#endif
#endif

#endif

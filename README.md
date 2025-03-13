# Furtive Squash

An example project for driving an SX1262 LoRa radio with an STM32 Nucleo board.

<!-- cSpell:words semtech arduino tcxo waveshare gpio mosi miso eabi muxed clangd -->

## Motivation

The [SX1262](https://www.semtech.com/products/wireless-rf/lora-connect/sx1262) by Semtech is a radio chip capable of receiving and transmitting LoRa, a low-power long-range radio protocol.

As an aside, note that a lot of items which come up if you search "LoRa module" online are based on Semtech's `SX1278` (or another in the `SX127x` series). Take note that even though the first two letters are the same and the number afterwards is higher, this is an **older** chip than the `SX1262`. The `*7x` series seem badly-supported, and I failed to get one to work the way I expected. If you don't know which one you want, I strongly recommend going for an `SX1262` over an `SX127x`.

Having access to a custom LoRa radio would be very useful for hobby projects, such as making an "internet of things" cluster of devices without relying on off-the-shelf devices of varying quality and cost.

This project uses an [STM32F756 NUCLEO](https://www.st.com/en/evaluation-tools/nucleo-f756zg.html) to drive the radio chip. This is a development board with the powerful (for a microcontroller) `STM32F756ZGT6` as the microcontroller, in the Nucleo-144 form-factor (the Nucleo-32 is the size of an Arduino Nano and Nucleo-64 is the size of an Arduino Uno, for comparison).

The particular SX1262 module I'm using is the `Core1262-868M` by Waveshare, but if the schematic they provide is to believe, it basically just has a TCXO and some passives under the can, so you can basically treat it as a break-out for the SX1262.

## Licencing

The code here, as per the `LICENSE` file, is provided under the permissive MIT licence.

It requires external libraries, which are in the `third_party` directory as Git submodules. They have their own licences, which you need to comply with if you use this project.

Beyond that, you have my permission to use anything here which was written by me for whatever you want, with no need for attribution or compensation.

## Hardware configuration

The STM32 code always references pins by their GPIO bank and pin, e.g. `PA1` is pin 1 on GPIO bank A.

However, the silkscreen on the unit (the text written on the board) use the Arduino naming conventions, e.g. `D1` for digital pin 1. To get a mapping of one to the other, see the user manual for the nucleo (document ID UM1974), figure 11.

Here's my wiring:

Function | Colour | Arduino | STM32
:---     | :---   | :---    | :---
CS       | Yellow | D8      | PF12
CLK      | Green  | D13     | PA5
MOSI     | Blue   | D11     | PA7
MISO     | Purple | D12     | PA6
DIO2     | Orange | D4      | PF14
DIO1     | Brown  | D7      | PF13
RST      | Grey   | D5      | PE11
BUSY     | White  | D6      | PE9
3V3      | Red    | 3V3     | -
GND      | Black  | GND     | -

The `SX1262` board came with holes/pads which can be soldered, so I soldered male dupont headers onto it, then used dupont jumpers to connect it to the Nucleo.

The only other things you'll need are a micro-USB cable to connect to the Nucleo's ST-Link (make sure you use the right one - `CN1`, not `CN13`), and an antenna. The Waveshare board has a U.FL connector, but most antennas suitable for 868MHz have an SMA connector, so consider getting an adaptor. There's also a pad on the board for an antenna, so you can solder one instead of using a connector.

## Building and programming

Build the binary by running `make` at the top level of this repository, making sure you have Arm GCC (`arm-none-eabi-`) on the path, or use flag `GCC_PATH=` to point to your toolchain.

Currently, the code is written to run in either "RX" or "TX" mode. To make best use of it, configure two identical units and build the app for one using `make RX_TX_MODE=RX`, and the other using `make RX_TX_MODE=TX`. The `RX` board will listen for packets in a loop, and the `TX` board will send packets in a loop.

The third party libraries which are used (e.g. the STM32 HAL) are added as submodules (make sure you do `git submodule update --init` to set them up) have a makefile each in `third_party/`, and they compile to a statically-linked archive (a `*.a` file), which the main library links against. This provides clear separation between the code here and the code from elsewhere.

To program the binary onto the board, if you have `openocd` installed, you can run `make flash` to write the program.

If you have the `screen` utility installed, you can also run `make serial` (preferably in a separate terminal) to view the UART prints from the unit (the ST-Link USB connection also has a serial interface muxed into it).

## Development environment

This project was set up by initially using ST's "CubeMX" tool, which can generate an example app for a particular MCU (or, if you have an official ST board like the Nucleo, you can use the board to set the options rather than having to configure everything yourself).

Note that the Cube generates horrible code, with terrible structure and lots of ugly "USER CODE BEGIN/END" tags to let you regenerate code without wiping out your custom code.

I rearranged things, and created proper separation between user-owned code and third-party code.

The code editor used was VS Code (Microsoft should never be trusted - for example, they recently quietly removed the "download extension" link on marketplace pages, to make it harder to download them for offline use - so use it at your own risk, and consider getting a completely FOSS alternative like [VSCodium](https://vscodium.com/)). The recommended extensions (listed in `.vscode/extensions.json`) are `clangd` for C/C++ parsing, which is much better performing than the MS C/C++ extension, and "Code Spell Checker" (if you see "cSpell:words" comments, those are lists of exceptions for the spell-checker).

The project makes use of the "STM HAL" functions. These are relatively user-friendly, but often have a lot of unnecessary code (e.g. handling UART with every possible word size, even if you're using the simplest 8-bit-word case), and can create their own problems (e.g. you can't use the functions `HAL_UART_Transmit()` and `HAL_UART_Receive()` simultaneously, even though it's perfectly possible to do so with the hardware, as the HAL "locks" the peripheral).

Instead of using the HAL functions, consider using the low-level (LL) drivers (for example, use `stm32f7xx_ll_usart.h` instead of `stm32f7xx_hal_usart.h`). The LL drivers are much faster than the HAL functions, at the cost of having to understand the underlying hardware a little better.

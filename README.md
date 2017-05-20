# README

This repository contains a selftest firmware for the 1Bitsy STM32F415RGT6 dev board.

This firmware uses the JTAG/SWD interface to report the test result back.

## Usage

You will have to fetch the libopencm3 submodule by running:

    git submodule init
    git submodule update

You compile the needed part of the library and the project firmware by invoking
"make" in the toplevel directory.

Executing "make flash" will try to use arm-none-eabi-gdb to connect ta a Black
Magic Probe and upload the firmware to your target.

## Contributions

Pull requests simplifying and making this example easier to adapt to other
platforms and projects are welcome! Please strive to keep things fairly simple
and magical as possible. :)

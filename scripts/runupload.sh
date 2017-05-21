#!/bin/bash

while true; do
	sleep 1;
	arm-none-eabi-gdb --batch -nx -ex "tar ext /dev/ttyACM0" -x scripts/gdb_upload.scr src/1bitsy_selftest.elf;
done

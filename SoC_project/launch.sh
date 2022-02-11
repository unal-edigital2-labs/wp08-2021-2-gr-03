#!/bin/bash

./load.py
echo 'Load SOC done'
echo '================================'
cd firmware
make clean
make all
echo 'make done'
echo '================================'
cd .. 
litex_term /dev/ttyUSB5 --kernel firmware/firmware.bin
echo 'Load firmware done'

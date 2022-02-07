#!/bin/bash

./load.py
echo 'Load done'
echo '================================'
cd firmware
make clean
make all
echo 'make done'
echo '================================'
cd .. 
litex_term.py /dev/ttyUSB1 --kernel firmware/firmware.bin

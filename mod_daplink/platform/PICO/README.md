# PICO rp2040

This is the _daplink_ module for micropython on raspberry pico. 

## Description
Pinout is in the file _IO_config.h_

- one CDC serial for micropython prompt
- one HID interace with CMSIS-DAP v1 adapter

## Building

These are instructions for building micropython with the _daplink_ module.

```bash
git clone  https://github.com/koendv/mod_daplink/
git clone https://github.com/micropython/micropython
cd micropython
patch -p1 < ../mod_daplink/mod_daplink/platform/PICO/upy_1CDC_1HID.patch
cd mpy-cross/
make
cd ../ports/rp2/
make submodules
make BOARD=PICO USER_C_MODULES=~/src/mod_daplink/micropython.cmake
```
To get the raspberry pico bootloader, press button _bootsel_ when pluggin in in usb, or type at the console prompt:
```python
>>> machine.bootloader()
```
The raspberry pico reboots, and a virtual disk _RPI-RP2_ connects. To upload firmware, copy the file _ports/rp2/build-PICO/firmware.uf2_ to the raspberry pi virtual disk.

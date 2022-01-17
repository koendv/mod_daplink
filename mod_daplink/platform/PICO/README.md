## Description

This is the _daplink_ module for micropython on raspberry pico. 
Pinout is in the file _IO_config.h_

- one CDC serial for micropython prompt
- one CDC serial, free for application use
- one HID interace with CMSIS-DAP v1 adapter

## Building

These are instructions for building micropython with the _daplink_ module.

```bash
git clone  https://github.com/koendv/mod_daplink/
git clone https://github.com/micropython/micropython
cd micropython
patch -p1 < ../moddap/moddap/platform/PICO/upy_2CDC_1HID.patch
cd mpy-cross/
make
cd ../ports/rp2/
make submodules
make BOARD=PICO USER_C_MODULES=~/src/moddap/micropython.cmake
```
To get the raspberry pico bootloader, press button _bootsel_ when pluggin in in usb, or type at the console prompt:
```python
>>> machine.bootloader()
```
The raspberry pico reboots, and a virtual disk _RPI-RP2_ connects. To upload firmware, copy the file _firmware.uf2_ to the raspberry pi virtual disk.

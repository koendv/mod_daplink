# PYBD_SF2
This is the _daplink_ module for micropython on Pyboard D-series. 

## Description

To run CMSIS-DAP, the following changes have been made to micropython:

- `mpconfigboard.h`: adding "CMSIS-DAP" to the usb product name. This needed for software like openocd and pyocd to find the board
- `pins.csv`: setting pins used as JTAG and SWD adapter
- `boot.py`:  set up the usb as a HID interface for CMSIS-DAP v1 
```
pyb.usb_mode('VCP+HID', hid=daplink.hid_info)
```
- `cmsis_dap.py`: the cmsis-dap adapter is implemented as a micropython loop.
```
    usb_hid=pyb.USB_HID()
    while(1):
        if (usb_hid.recv(app_request) and (daplink.process_request(app_request, app_response))[1] != 0):
            usb_hid.send(app_response)

```

## Building

```
git clone https://github.com/koendv/mod_daplink
git clone --recursive https://github.com/micropython/micropython
cd micropython/mpy-cross
make
cd ../ports/stm32/
patch -p1 < ~/src/mod_daplink/mod_daplink/platform/PYBD_SF2/micropython.patch
make submodules
make BOARD=PYBD_SF2 USER_C_MODULES=~/src/mod_daplink/
```
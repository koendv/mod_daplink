# boot.py -- run on boot to configure USB and filesystem
# Put app code in main.py

import pyb
import sys
import os
import machine
if pyb.SDCard().present():
    os.mount(pyb.SDCard(), '/sd')
    sys.path[1:1] = ['/sd', '/sd/lib']

pyb.country('US') # ISO 3166-1 Alpha-2 code, eg US, GB, DE, AU
#pyb.main('main.py') # main script to run after this one
#pyb.usb_mode('VCP+MSC') # act as a serial and a storage device
#pyb.usb_mode('VCP+HID') # act as a serial device and a mouse

import daplink
pyb.usb_mode('VCP+MSC+HID', msc=(pyb.Flash(), pyb.SDCard()), hid=daplink.hid_info) # act as serial device, storage device and cmsis-dap v1 hid device

# not truncated

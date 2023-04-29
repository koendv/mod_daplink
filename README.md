# module daplink
CMSIS-DAP is a protocol to access the debug interface of ARM processors.
This software implements CMSIS-DAP as a micropython module. The CMSIS-DAP interface can be acessed in two ways

- from pyocd and openocd software running on the pc host, accessing CMSIS-DAP as a usb hid device
- from a micropython script running on the CMSIS-DAP adapter itself, accessing CMSIS-DAP as a micropython module.

Compiled firmware for raspberry pico is available for download. The raspberry pico firmware has

- one usb cdc serial for micropython prompt
- one usb hid interface for CMSIS-DAP

micropython and CMSIS-DAP adapter can be active at the same time. 

## Compiling

As an example, compiling for a PYBD_SF2:

```
cd ~/src
git clone https://github.com/koendv/mod_daplink/
git clone http://github.com/micropython/micropython
cd micropython
patch -p1 < ~/src/mod_daplink/mod_daplink/platform/PYBD_SF2/micropython.patch
cd ports/stm32
make BOARD=PYBD_SF2 USER_C_MODULES=~/src/mod_daplink/
```

## micropython

The module _daplink_ implements the function _process_request_. The function _process_request_ has two parameters, _request_ and _response_. _request_ and _response_ are string or bytes, each 64 bytes long. The output of _process_request_ is a tuple of two numbers. The first number is the number of bytes read from _request_; the second number is the number of bytes written to _response_. If the number of bytes written to _response_ is zero, no response needs to be sent.

```python
>>> import daplink
>>> request=b'\x00\x02'+b'\x00'*62
>>> response=bytes(64)
>>> daplink.process_request(request,response)
(2, 10)
>>> print(response.decode('ascii'))
DAPLink
>>>
```
## CMSIS-DAP adapter

On raspberry pico CMSIS-DAP is implemented as a HID device, so you can use your raspberry pico as a CMSIS-DAP adapter, while running micropython code at the same time. You can use

```bash
$ pyocd-tool list
## => Board Name | Unique ID
-- -- ----------------------
 0 => MicroPython CMSIS-DAP | e6603828237f2935
$ sudo pyocd-gdbserver
```
and
```bash
$ sudo /usr/bin/openocd -f interface/cmsis-dap.cfg -f target/stm32f1x.cfg
```

## Firmware
Compiled firmware for raspberry pico in 'Releases'.

_not truncated_

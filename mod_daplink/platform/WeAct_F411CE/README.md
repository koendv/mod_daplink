# WeAct STM32F4x1 MiniF4
- [product](https://aliexpress.com/item/1005001456186625.html)
- [git](https://github.com/WeActTC/MiniSTM32F4x1)
- available ram in micropython: 83 kbyte, available flash for filesystem: 48 kbyte.
- WeAct_F411CE with 8Mbyte spi flash preferred; else little room for scripts.
- because of limited number of usb endpoints, only usb mode 'VCP+HID' can be used. 'VCP+MSC+HID' does not work.
- cmsis-dap adapter requires micropython scripts in modules/ directory.

## build
```
git clone https://github.com/micropython/micropython
cd micropython/ports/stm32/boards/
git clone https://github.com/WeActTC/WeAct_F411CE-MicroPython.git WeAct_F411CE
cd  WeAct_F411CE
patch -p1 < ~/src/mod_daplink/mod_daplink/platform/WeAct_F411CE/micropython-WeAct_F411CE-no-spi-flash.patch
cd ../..
make BOARD=WeAct_F411CE USER_C_MODULES=~/src/mod_daplink/
make BOARD=WeAct_F411CE USER_C_MODULES=~/src/mod_daplink/ deploy
```

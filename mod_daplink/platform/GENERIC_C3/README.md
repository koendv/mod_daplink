# ESP32-C3-Mini-1

* This is work in progress *

This is the _daplink_ module for micropython on the ESP32-C3-Mini-1, a risc-v board.

## Build
In DAP.h, change:
```
diff --git a/mod_daplink/DAPLink/source/daplink/cmsis-dap/DAP.h b/mod_daplink/DAPLink/source/daplink/cmsis-dap/DAP.h
index 02286de..83d97ae 100644
--- a/mod_daplink/DAPLink/source/daplink/cmsis-dap/DAP.h
+++ b/mod_daplink/DAPLink/source/daplink/cmsis-dap/DAP.h
@@ -334,7 +334,7 @@ extern void     DAP_Setup (void);
 #ifndef DELAY_SLOW_CYCLES
 #define DELAY_SLOW_CYCLES       3U      // Number of cycles for one iteration
 #endif
-#if defined(__CC_ARM)
+#if defined(__CC_ARM) || !defined(__arm__)
 __STATIC_FORCEINLINE void PIN_DELAY_SLOW (uint32_t delay) {
   uint32_t count = delay;
   while (--count);
```
To build the firmware:

```
cd src
git clone -b release/v4.3 --recursive  https://github.com/espressif/esp-idf/
cd esp-idf/
./install.sh esp32c3
source export.sh
git clone --recursive https://github.com/micropython/
cd micropython/mpy-cross
make
cd ../ports/esp32/
make BOARD=GENERIC_C3 USER_C_MODULES=~/src/mod_daplink/micropython.cmake deploy
```
Boot prompt:
```
MicroPython v1.18-10-g5db278f1d on 2022-01-20; ESP32C3 module with ESP32C3
```


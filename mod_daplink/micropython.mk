DAPLINK_MOD_DIR := $(USERMOD_DIR)

SRC_USERMOD += \
	$(DAPLINK_MOD_DIR)/mod_daplink.c \
	$(DAPLINK_MOD_DIR)/DAPLink/source/daplink/cmsis-dap/DAP.c \
	$(DAPLINK_MOD_DIR)/DAPLink/source/daplink/cmsis-dap/SW_DP.c \
	$(DAPLINK_MOD_DIR)/DAPLink/source/daplink/cmsis-dap/JTAG_DP.c \
	$(DAPLINK_MOD_DIR)/platform/$(BOARD)/info.c \

CFLAGS_USERMOD += \
	-I$(DAPLINK_MOD_DIR) \
	-I$(DAPLINK_MOD_DIR)/DAPLink/source/daplink/cmsis-dap \
	-I$(DAPLINK_MOD_DIR)/DAPLink/source/daplink/ \
	-I$(DAPLINK_MOD_DIR)/DAPLink/source/cmsis-core \
	-I$(DAPLINK_MOD_DIR)/platform/$(BOARD) \
	-Wno-unused-variable \

#not truncated

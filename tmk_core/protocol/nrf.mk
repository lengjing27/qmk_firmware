# for protocol specific files

PROTOCOL_DIR = protocol
NRF_DIR = $(PROTOCOL_DIR)/nrf


SRC += $(NRF_DIR)/main.c

VPATH += $(TMK_PATH)/$(PROTOCOL_DIR)
VPATH += $(TMK_PATH)/$(NRF_DIR)
VPATH += $(TMK_PATH)/$(NRF_DIR)/lufa_utils

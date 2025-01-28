TARGET?=ESP32
#TARGET?=ProMini

PORT?=/dev/ttyUSB0
ifeq ("$(TARGET)", "ESP32")
ESP32_FILESYSTEM=spiffs
ESP32_FILESYSTEM_PART=spiffs
ESP32_PARTSCHEME=min_spiffs
ESP32_FLASHSIZE=4MB
else ifeq ("$(TARGET)", "ProMini")
endif

include ../Arduino.mk

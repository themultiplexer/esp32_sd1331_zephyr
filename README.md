# Zephyr ESP32 Demo with SD1331 display

## Building/Flashing for ESP32 (ESP32-WROOM-32, ...)

1. `west build -b esp32_devkitc/esp32/procpu esp32_sd1331_zephyr`
2. `west flash --esp-device /dev/ttyACM0 -- --esp-baud-rate 115200`
3. `west espressif monitor`


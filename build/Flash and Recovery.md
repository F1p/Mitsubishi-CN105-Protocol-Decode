# Hardware

Supported Hardware is ESP8266 ebay sold hardware: https://www.ebay.co.uk/itm/325967595655



# ESP8266 Ebay Hardware Recovery (Gen1)


1. Download the files from the [/build](https://github.com/F1p/Mitsubishi-CN105-Protocol-Decode/tree/master/build/esp8266.esp8266.generic/v5.2.5) folder.

2. Download [upload.py](https://github.com/esp8266/Arduino/blob/master/tools/upload.py) from Github

3. Place all the files in the same folder

4. Disconnect from CN105, Unclip the lid, slide out the circuit board

5. Plug the pins into the programming board, #both USB ports at the same side

6. Connect the USB cable to the bottom board (programming carrier board), locate the COM port your device is

7. Hold the flash button down, while holding then press "reset". You can then release both buttons.

8. Open CMD prompt in the folder with the files saved and run:

    ```
    python upload.py --chip esp8266 --port "COM4" --baud "115200" "" erase_flash --before default_reset --after hard_reset write_flash 0x0 "ECODAN_Bridge_v5.2.5.bin"
    ```




# M5Stack Atom S3 Lite (Gen2)

1. Download the files from the [/build](https://github.com/F1p/Mitsubishi-Ecodan-Bridge-CN105/tree/master/build/esp32.esp32.m5stack-atoms3) folder.

2. Download [esptool.exe](https://github.com/espressif/esptool/releases) from Espressif.

3. Place all the files in the same folder, press and hold the side button of the M5Stack, locate the COM port your M5Stack is on

4. Open CMD prompt in the folder with the files saved and run:

    ```
    esptool.exe --chip esp32s3 --port "COM12" --baud 921600  --before default_reset --after hard_reset write_flash -e -z --flash_mode dio --flash_freq 80m --flash_size 4MB 0x0 "ECODAN_Bridge_v5.2.0.bootloader.bin" 0x8000 "ECODAN_Bridge_v5.2.0.partitions.bin" 0xe000 "boot_app0.bin" 0x10000 "ECODAN_Bridge_v5.2.0.bin"
    ```

5. The Pins on the M5Stack are: 1 = Tx, 2 = Rx




# Wemos D1 Mini

1. Download the files from the [/build](https://github.com/F1p/Home-Assistant-Mitsubishi-Ecodan-CN105-to-MQTT/tree/master/build/esp8266.esp8266.d1_mini/v5.2.1) folder.

2. Download [upload.py](https://github.com/esp8266/Arduino/blob/master/tools/upload.py) from Github

3. Place all the files in the same folder, locate the COM port your Wemos D1 Mini is on

4.  Open CMD prompt in the folder with the files saved and run:

    ```
    python upload.py --chip esp8266 --port "COM4" --baud "115200" "" erase_flash --before default_reset --after hard_reset write_flash 0x0 "ECODAN_Bridge_v5.2.1_WemosD1.bin"
    ```

5. The Pins on the Wemos D1 Mini are: D0 = Tx, D5 = Rx

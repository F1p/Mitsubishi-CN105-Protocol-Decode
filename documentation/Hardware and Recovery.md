# Hardware and Recovery Information



M5Stack Atom S3 Lite

1. Download the files from the [/build]([https://github.com/F1p/Mitsubishi-Ecodan-Bridge-CN105/tree/master/build/esp32.esp32.m5stack-atoms3](https://github.com/F1p/Mitsubishi-CN105-Protocol-Decode/tree/master/build/esp32.esp32.m5stack_atoms3/v5.3.0%20Beta)) folder.

2. Download [esptool.exe](https://github.com/espressif/esptool/releases) from Espressif (4.9.dev3).

3. Place all the files in the same folder, press and hold the side button of the M5Stack, locate the COM port your M5Stack is on

4. Open CMD prompt in the folder with the files saved and run:

    ```
    esptool.exe --chip esp32s3 --port "COM63" --baud 921600  --before default_reset --after hard_reset write_flash -e -z --flash_mode keep --flash_freq keep --flash_size keep 0x0 "ECODAN_Bridge_Gen2_v6.1.2.bootloader.bin" 0x8000 "ECODAN_Bridge_Gen2_v6.1.2.partitions.bin" 0xe000 "boot_app0.bin" 0x10000 "ECODAN_Bridge_Gen2_v6.1.2.bin" 
    ```

5. The Pins on the M5Stack are: 1 = Tx, 2 = Rx


Wemos D1 Mini

1. Download the files from the [/build](https://github.com/F1p/Home-Assistant-Mitsubishi-Ecodan-CN105-to-MQTT/tree/master/build/esp8266.esp8266.d1_mini/v5.2.1) folder.

2. Download [upload.py](https://github.com/esp8266/Arduino/blob/master/tools/upload.py) from Github

3. Place all the files in the same folder, locate the COM port your Wemos D1 Mini is on

4.  Open CMD prompt in the folder with the files saved and run:

    ```
    python upload.py --chip esp8266 --port "COM4" --baud "115200" "" erase_flash --before default_reset --after hard_reset write_flash 0x0 "ECODAN_Bridge_v5.2.1_WemosD1.bin"
    ```

5. The Pins on the Wemos D1 Mini are: D0 = Tx, D5 = Rx
# Portable Wireless Interrogation System For Magnetoelastic Biosensors

This portable wireless interrogation system allows for the real-time detection and monitoring of Magnetoelastic sensors and uses the “Ringdown” method of detection using a Transmit side and a Receive side. The Ringdown Analysis method characterizes an ME sensor by looking at its vibration duration vs frequency of a time-varying magnetic field. 

The objectives of the code in this repo is to send BLE packets of the generated data over to a Bluetooth capable device to be viewed using a HTML webapp and graph the results obtained from the sensor readout system on a Touch Screen that has an interactive GUI to set/change system control parameters.

[Link to webapp](https://prasana-sys.github.io/Magnetoelastic_Biosensor_PWIS/)

## Parts Used
- Silicon Labs BGM220P Explorer Kit / xG24 Explorer Kit
- Adafruit RA8875 TFT Driver
- Adafruit 7" 800x480 Touch Screen

## System Design

The system is split into two parts, the STM32 and the Si Labs microcontroller side. They communicate over UART to send two different kinds of packets, ringdown data and ringdown parameters.

![System Design](img/ME_sensor_PWIS.drawio.png)

When the BGM220P boots, the BLE stack initializes a GATT database that includes a custom ME sensor service with data characteristics for sending sensor readings via indications and receiving the parameters to/from a cell phone or laptop using a HTML based app.

![BLE GATT PROFILE](img/BLE%20GATT%20Profile.png)

More info in doc/PWIS_Expo_Poster

## Development Environment

- Arduino IDE Version: 2.3.5
- Si Labs Arduino Core Version: 2.3.0
- Adafruit GFX Library Version: 1.12.0
- Adafruit RA8875 Library Version: 1.4.4

Follow Si Labs Arduino Core installation and bootloader burn procedures to instantiate a new Si Labs microcontroller.

- [Si Labs Arduino Core Installation](https://github.com/SiliconLabs/arduino?tab=readme-ov-file#installation)

- [Si Labs Arduino Core Bootloader](https://github.com/SiliconLabs/arduino?tab=readme-ov-file#getting-started)

Main .ino script is in the root folder with this README. Other cpp and header files are included in src/. The file structure in src/ is to be maintained for Arduino IDE to know which files to compile and link to the main script when verifying and uploading to target device. Those cpp and header files can also be placed in the root folder for easier development if needed.

Also take a look /doc/xG24_pinout_bug.md for important information.
# Ambient Light Sensor
## General info
BHI750VDI sensor controlled by Arduino Uno. Lets you check light level in the room and save the results to EEPROM (electrically erasable programmable read-only memory) on Arduino.

## Technologies
Project was created with Atmel Studio 7.0.

## How it works
After successful setup system automatically sends measured value (in luxes) to the USART (Universal Synchronous/Asynchronous Receiver/Transmitter) controlled console. New result is shown every 120 ms as this is how much the sensor needs to actualise. Right after every measurement the result is saved in EEPROM on Arduino. 

Code is written in pure C, without Arduino libraries. Arduino and light sensor communicate using IIC (Inter-Integrated Circuit)/TWI (Two-Wire Interface).

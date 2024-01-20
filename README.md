# GAS_DETECTOR
Gas Detection project I made while attending NAIT

This includes the PCB and circuit design I made using KiCad EDA, as well as the software to run the system using C in the Microchip Studio IDE by Microchip Technology. Credit to Simon Walker for some of the libraries.

Here's how it works:

-Using a gas sensor to detect smoke/gas in PPM (parts per million, standard for measuring the volume of a gas)
-Once the sensor detects a certain amount of gas (about 200PPM, and displayed on an I2C supported LCD), an “alarm” activates (piezo buzzer)
-The LCD will display the current volume of gas detected by the gas sensor in PPM
-Push button for Reset

Involves the following components:

-MQ-2 gas sensor, widely used with lots of resources/examples to look at, works at 5V (https://components101.com/sensors/mq2-gas-sensor) https://media.digikey.com/pdf/Data%20Sheets/Seeed%20Technology/101020055_Web.pdf
-LCD w/ I2C: AMC0802BR-B-Y6WFDY-I2C (https://www.digikey.ca/en/products/detail/orient-display/AMC0802BR-B-Y6WFDY-I2C/12089323), cheap on digikey, runs at 5V as well. 0111110 = I2C address 
-BH9VPC 9V battery holder (had this on hand) + LM7805 linear voltage regulator to get the 5V needed for everything
-ATmega328P for the microcontroller https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
-Piezo Buzzer for the alarm (PS1240P02BT) https://www.digikey.ca/en/products/detail/tdk-corporation/PS1240P02BT/935924, can run on 3V minimum up to a maximum of 30Vp
-A push button

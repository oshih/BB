BBBiolib
=======

Beaglebone black I/O library , using Memory mapped I/O

Simple C I/O library of Beaglebone black

============================

*This is a clone of the BBBiolib originally developed by Meng-Lun Cai (Vegetable Avenger) at [https://github.com/VegetableAvenger/BBBIOlib](https://github.com/VegetableAvenger/BBBIOlib)*

Changes would be made to make it compatible with Linux 4.0+

### Changes
* Added demo `sound capture with ADC` using the Roseline board (MEMS Microphone with fixed gain amp on AIN2)
* Added Makefile for compiling other applications

**How to use :**

        Build libBBBio.a :

                # make

        Build Demos : (ex : ADT7301 demo)

                # make ADT7301


**Toolkit List :**

	1. GPIO CLK Status toolkit

	2. Expansion Header toolkit

	3. ADC Argument Calculation toolkit


**Demo List :**

	1. LED Demo

	2. LED_GPIO Demo

	3. ADT7301 Demo (Digital Temperature sensor)

	4. Seven-Segment Array Display demo

	5. Debouncing Demo

	6. 4x4 keypad Demo

	7. PWM Demo

	8. Servo Motor Demo

	9. ADC Demo

	10. ADC with Arduino Microphone Demo

	11. L3G4200D Demo (3-axis Gyroscope)


**overlays**

BBBIO include some device tree overlays example for linux kernel 3.8.13 .

compile each overlays:

	#cd overlay

	#make

Install overlays

	#cd overlay

	#make install

apply overlay :

	#cp {OVERLAY} /lib/firmware/

	#echo {OVERLAY's part-number} >> /sys/devices/bone_capemgr.*/slots

overlays list :

	1. EHRPWM 0~3 overlay

		#echo BBBIO-EHRPWM >> /sys/devices/bone_capemgr.*/slots

	2. SPI 1 overlays

		#echo BBBIO-SPI1 >> /sys/devices/bone_capemgr.*/slots

Reference from :

	https://learn.adafruit.com/introduction-to-the-beaglebone-black-device-tree/overview



**External Reference**

Bulldog :

	Bulldog is a GPIO API for the Beaglebone Black written in Java.

	github : https://github.com/Datenheld/Bulldog

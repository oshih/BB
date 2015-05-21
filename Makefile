
#---------------------------------------------------
# Paths etc.
#---------------------------------------------------
LIB_PATH = ./BBBio_lib
DEMO_PATH = ./Demo
TOOLKIT_PATH = ./Toolkit
LAB_PATH = ./Lab


LIBRARY = BBBio

#---------------------------------------------------
# Compiler options
#---------------------------------------------------

CC_PATH = 
CC_PREFIX = 
CC = ${CC_PATH}/${CC_PREFIX}gcc
AR = ${CC_PATH}/${CC_PREFIX}ar

# Display information
help : 
	@echo "BBBio Makefile help"
	@echo
	@echo "The library can be used the following way:"
	@echo "$ make <command>"
	@echo
	@echo "help, all or <blank>: show this prompt"
	@echo "install: create BBBio.a"
	@echo "clean: clean library (useful if you changed something)"
	@echo "demo name: compile a particular demo"

all: help

#---------------------------------------------------
# Compile Library
#---------------------------------------------------
libBBBio.a : ${LIB_PATH}BBBiolib.c ${LIB_PATH}BBBiolib.h BBBiolib_PWMSS.o BBBiolib_McSPI.o BBBiolib_ADCTSC.o
	${CC} -c ${LIB_PATH}BBBiolib.c -o ${LIB_PATH}BBBiolib.o
	${AR} -rs ${LIB_PATH}libBBBio.a ${LIB_PATH}BBBiolib.o ${LIB_PATH}BBBiolib_PWMSS.o ${LIB_PATH}BBBiolib_McSPI.o ${LIB_PATH}BBBiolib_ADCTSC.o
#	cp ${LIB_PATH}libBBBio.a ./

BBBiolib_PWMSS.o : ${LIB_PATH}BBBiolib_PWMSS.c ${LIB_PATH}BBBiolib_PWMSS.h
	${CC} -c ${LIB_PATH}BBBiolib_PWMSS.c -o ${LIB_PATH}BBBiolib_PWMSS.o -W 

BBBiolib_McSPI.o : ${LIB_PATH}BBBiolib_McSPI.c ${LIB_PATH}BBBiolib_PWMSS.h
	${CC} -c ${LIB_PATH}BBBiolib_McSPI.c -o ${LIB_PATH}BBBiolib_McSPI.o -W

BBBiolib_ADCTSC.o : ${LIB_PATH}BBBiolib_ADCTSC.c ${LIB_PATH}BBBiolib_ADCTSC.h
	${CC} -c ${LIB_PATH}BBBiolib_ADCTSC.c -o ${LIB_PATH}BBBiolib_ADCTSC.o -W


install : libBBBio.a 
	

#---------------------------------------------------
# Compile Applications
#---------------------------------------------------

LED : ${DEMO_PATH}/Demo_LED/LED.c libBBBio.a
	${CC} -o LED ${DEMO_PATH}/Demo_LED/LED.c -L ${LIB_PATH} -l${LIBRARY}

ADT7301 : ${DEMO_PATH}/Demo_ADT7301/ADT7301.c libBBBio.a
	${CC} -o ADT7301 ${DEMO_PATH}/Demo_ADT7301/ADT7301.c -L ${LIB_PATH} -l${LIBRARY}

ADXL345 :  ${DEMO_PATH}/Demo_ADXL345/ADXL345.c libBBBio.a
	${CC} -o ADXL345 ${DEMO_PATH}/Demo_ADXL345/ADXL345.c -L ${LIB_PATH} -l${LIBRARY}
ADXL345_NET :  ${DEMO_PATH}/Demo_ADXL345/ADXL345_net.c libBBBio.a
	${CC} -o ADXL345 ${DEMO_PATH}/Demo_ADXL345/ADXL345_net.c -L ${LIB_PATH} -l${LIBRARY}

L3G4200D : ${DEMO_PATH}/Demo_L3G4200D/L3G4200D.c libBBBio.a
	${CC} -o L3G4200D ${DEMO_PATH}/Demo_L3G4200D/L3G4200D.c -L ${LIB_PATH} -l${LIBRARY}

SEVEN_SCAN : ${DEMO_PATH}/Demo_SevenScan/SevenScan.c libBBBio.a
	${CC} -o SevenScan ${DEMO_PATH}/Demo_SevenScan/SevenScan.c  -L ${LIB_PATH} -l${LIBRARY}

SMOTOR : ${DEMO_PATH}/Demo_ServoMotor/ServoMotor.c libBBBio.a
	${CC} -o SMOTOR ${DEMO_PATH}/Demo_ServoMotor/ServoMotor.c -L ${LIB_PATH} -l${LIBRARY}

LED_GPIO : ${DEMO_PATH}/Demo_LED_GPIO/LED_GPIO.c libBBBio.a
	${CC} -o LED_GPIO ${DEMO_PATH}/Demo_LED_GPIO/LED_GPIO.c -L ${LIB_PATH} -l${LIBRARY} -pthread

DEBOUNCING : ${DEMO_PATH}/Demo_Debouncing/Debouncing.c libBBBio.a
	${CC} -o Debouncing ${DEMO_PATH}/Demo_Debouncing/Debouncing.c -L ${LIB_PATH} -l${LIBRARY}

4x4keypad : ${DEMO_PATH}/Demo_4x4keypad/4x4keypad.c libBBBio.a
	${CC} -o 4x4keypad ${DEMO_PATH}/Demo_4x4keypad/4x4keypad.c -L ${LIB_PATH} -l${LIBRARY}

PWM : ${DEMO_PATH}/Demo_PWM/PWM.c libBBBio.a
	${CC} -o PWM ${DEMO_PATH}/Demo_PWM/PWM.c -L ${LIB_PATH} -l${LIBRARY}

BMOTOR : ${DEMO_PATH}/Demo_BrushlessMotor/BrushlessMotor.c libBBBio.a
	 ${CC} -o BMOTOR ${DEMO_PATH}/Demo_BrushlessMotor/BrushlessMotor.c -L ${LIB_PATH} -l${LIBRARY}

ADC : ${DEMO_PATH}/Demo_ADC/ADC.c libBBBio.a
	${CC} -o ADC ${DEMO_PATH}/Demo_ADC/ADC.c -L ${LIB_PATH} -l${LIBRARY} -lm

ADC_VOICE : ${DEMO_PATH}/Demo_ADC/ADC_voice.c libBBBio.a
	${CC} -o ADC_VOICE ${DEMO_PATH}/Demo_ADC/ADC_voice.c -L ${LIB_PATH} -l${LIBRARY} -lm -pthread -O3


DAC_VOICE : ${DEMO_PATH}/Demo_DAC/DAC_voice.c libBBBio.a
	${CC} -o DAC_VOICE ${DEMO_PATH}/Demo_DAC/DAC_voice.c -L ${LIB_PATH} -l${LIBRARY} -lm -pthread -O3

Sound_Capture_ADC : ${DEMO_PATH}/Demo_ADC/ADC.c libBBBio.a
	${CC} -o ADC ${DEMO_PATH}/Demo_Sound_Capture_ADC/Capture.c -L ${LIB_PATH} -I ${INCLUDE_PATH} -l${LIBRARY} -lm
#---------------------------------------------------
# toolkit 
#---------------------------------------------------

GPIO_STATUS : ${TOOLKIT_PATH}Toolkit_GPIO_CLK_Status/GPIO_status.c libBBBio.a
	${CC} -o GPIO_CLK_status ${TOOLKIT_PATH}Toolkit_GPIO_CLK_Status/GPIO_status.c  -L ${LIB_PATH} -l${LIBRARY}

EP_STATUS : ${TOOLKIT_PATH}Toolkit_EP_Status/EP_status.c libBBBio.a
	${CC} -o EP_status ${TOOLKIT_PATH}Toolkit_EP_Status/EP_status.c -L ${LIB_PATH} -l${LIBRARY}


ADC_CALC : ${TOOLKIT_PATH}Toolkit_ADC_CALC/ADC_CALC.c
	${CC} -o ADC_CALC ${TOOLKIT_PATH}Toolkit_ADC_CALC/ADC_CALC.c


#---------------------------------------------------
# Lab
#---------------------------------------------------

RA : ${LAB_PATH}Lab_Robot_Arm/Robot_Arm.c libBBBio.a
	${CC} -o RA  ${LAB_PATH}Lab_Robot_Arm/Robot_Arm.c -L ${LIB_PATH} -l${LIBRARY} -lm

VD : ${LAB_PATH}Voice_Door/voice_door.cpp libBBBio.a
	g++ -o VD ${LAB_PATH}Voice_Door/voice_door.cpp -L ${LIB_PATH} -l${LIBRARY} -lfftw3 -lm -pthread -O3


.PHONY: clean
clean :
	rm -rf ${LIB_PATH}*.o ${LIB_PATH}libBBBio.a libBBBio.a LED ADT7301 GPIO_CLK_status SevenScan Ultrasonic28015 TMP SMOTOR LED_GPIO Debouncing 4x4keypad EP_status PWM RA ADXL345 ADC ADC_CALC L3G4200D


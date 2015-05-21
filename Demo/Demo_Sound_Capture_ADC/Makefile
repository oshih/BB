#---------------------------------------------------
# Paths etc
#---------------------------------------------------
LIB_PATH = /root/BBBIOlib/BBBio_lib
INCLUDE_PATH = /root/BBBIOlib

# without .c extension
TARGET = Capture

#---------------------------------------------------
# Compiler options
#---------------------------------------------------

CC = gcc
AR = ar
LD = ld
LIBRARIES = BBBio

#---------------------------------------------------
# Compile Library
#---------------------------------------------------
${LIB_PATH}/libBBBio.a : ${LIB_PATH}/BBBiolib.c ${LIB_PATH}/BBBiolib.h BBBiolib_PWMSS.o BBBiolib_McSPI.o BBBiolib_ADCTSC.o
	${CC} -c ${LIB_PATH}/BBBiolib.c -o ${LIB_PATH}/BBBiolib.o
	${AR} -rs ${LIB_PATH}/libBBBio.a ${LIB_PATH}/BBBiolib.o ${LIB_PATH}/BBBiolib_PWMSS.o ${LIB_PATH}/BBBiolib_McSPI.o ${LIB_PATH}/BBBiolib_ADCTSC.o

BBBiolib_PWMSS.o : ${LIB_PATH}/BBBiolib_PWMSS.c ${LIB_PATH}/BBBiolib_PWMSS.h
	${CC} -c ${LIB_PATH}/BBBiolib_PWMSS.c -o ${LIB_PATH}/BBBiolib_PWMSS.o -W 

BBBiolib_McSPI.o : ${LIB_PATH}/BBBiolib_McSPI.c ${LIB_PATH}/BBBiolib_PWMSS.h
	${CC} -c ${LIB_PATH}/BBBiolib_McSPI.c -o ${LIB_PATH}/BBBiolib_McSPI.o -W

BBBiolib_ADCTSC.o : ${LIB_PATH}/BBBiolib_ADCTSC.c ${LIB_PATH}/BBBiolib_ADCTSC.h
	${CC} -c ${LIB_PATH}/BBBiolib_ADCTSC.c -o ${LIB_PATH}/BBBiolib_ADCTSC.o -W

install : libBBBio.a 
	

#---------------------------------------------------
# Compile application
#---------------------------------------------------


all : ${TARGET}.c ${LIB_PATH}/libBBBio.a
	${CC} -o ${TARGET}.out ${TARGET}.c -I ${INCLUDE_PATH} -L ${LIB_PATH} -l ${LIBRARIES} -lm

#---------------------------------------------------
# Clean
#---------------------------------------------------

.PHONY: clean
clean :
	rm -rf ${LIB_PATH}*.o ${LIB_PATH}/libBBBio.a libBBBio.a ${TARGET}.out


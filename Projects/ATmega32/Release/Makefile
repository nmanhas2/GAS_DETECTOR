###############################################################################
# Makefile for project 
###############################################################################

## General options
PROJECT = 
MCU = ATmega32
COMPILER = "WinAVR"
TARGET = Release
SHELL = C:\Windows\system32\cmd.exe

## Tools general options
CCFLAGS=-Wall -gdwarf-2 -fsigned-char -MD -MP -DF_CPU=1000000 -Os -mmcu=atmega32
LDFLAGS=-mmcu=atmega32

## Processing Tools
CC=avr-gcc.exe
ASM=avr-gcc.exe
LD=avr-gcc.exe
#  D:/Proteus_Work/Microcontrollers/atmega_microcontroller/gas/at32gas/ATmega32/Release/Release.hex: TOOL=LD
#  Post: avr-objcopy -O ihex -R .eeprom "./Release.elf" "./Release.hex"
#  Post: avr-objcopy -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 --no-change-warnings -O ihex "./Release.elf" "./Release.eep" || exit 0 
#     D:/Proteus_Work/Microcontrollers/atmega_microcontroller/gas/at32gas/ATmega32/Release/main.o: TOOL=CC
#        D:/Proteus_Work/Microcontrollers/atmega_microcontroller/gas/at32gas/ATmega32/main.c
#     D:/Proteus_Work/Microcontrollers/atmega_microcontroller/gas/at32gas/ATmega32/Release/lcd.o: TOOL=CC
#        D:/Proteus_Work/Microcontrollers/atmega_microcontroller/gas/at32gas/ATmega32/lcd.c
#     D:/Proteus_Work/Microcontrollers/atmega_microcontroller/gas/at32gas/ATmega32/Release/adc.o: TOOL=CC
#        D:/Proteus_Work/Microcontrollers/atmega_microcontroller/gas/at32gas/ATmega32/adc.c
#     D:/Proteus_Work/Microcontrollers/atmega_microcontroller/gas/at32gas/ATmega32/Release/mq2.o: TOOL=CC
#        D:/Proteus_Work/Microcontrollers/atmega_microcontroller/gas/at32gas/ATmega32/mq2.c

# Build tree:
all:	Release

Release:	Release.hex
	cp "Release.hex" "D:/Proteus_Work/Microcontrollers/atmega_microcontroller/gas/at32gas/ATmega32.hex"

Release.hex:	main.o lcd.o adc.o mq2.o 
	$(LD) $(LDFLAGS)  -o "./Release.elf" "main.o" "lcd.o" "adc.o" "mq2.o"
	avr-objcopy -O ihex -R .eeprom "./Release.elf" "./Release.hex"
	avr-objcopy -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 --no-change-warnings -O ihex "./Release.elf" "./Release.eep" || exit 0 

main.o:	../main.c 
	$(CC) $(CCFLAGS)  -o "main.o" -c "../main.c"

lcd.o:	../lcd.c 
	$(CC) $(CCFLAGS)  -o "lcd.o" -c "../lcd.c"

adc.o:	../adc.c 
	$(CC) $(CCFLAGS)  -o "adc.o" -c "../adc.c"

mq2.o:	../mq2.c 
	$(CC) $(CCFLAGS)  -o "mq2.o" -c "../mq2.c"

# tidy - delete all temporary files which are not involved in the target generation
tidy:
	rm -rf ./main.d
	rm -rf ./lcd.d
	rm -rf ./adc.d
	rm -rf ./mq2.d

# cleanup - delete all generated files
clean:	tidy
	rm -rf Release.hex
	rm -rf main.o
	rm -rf lcd.o
	rm -rf adc.o
	rm -rf mq2.o

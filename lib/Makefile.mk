# Arduino library makefile
# Jason Dreisbach
# Jason Descenzo

SRC += $(LIB)/lcd.c

# device info
MCU = atmega328p
F_CPU = 16000000
PORT = /dev/tty.usb*
ISP = arduino
UPLOAD_RATE = 115200

DEFINES = -DF_CPU=$(F_CPU)

all:
	avr-gcc -g -mmcu=$(MCU) $(DEFINES) -I . -I $(LIB) -Os -Werror -mcall-prologues -o $(TARGET).elf $(SRC) -lm
	avr-objcopy -O ihex $(TARGET).elf $(TARGET).hex
	avr-size $(TARGET).elf

install: all
	avrdude -p $(MCU) -P $(PORT) -c $(ISP) -u -U flash:w:$(TARGET).hex

clean:
	rm $(TARGET).elf
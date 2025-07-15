PRG = arduino
PRGPORT = /dev/ttyACM0
MCU     ?= atmega328p
SPEED   ?= 16000000L
CC      = avr-gcc
CFLAGS = -Os -flto -mmcu=$(MCU) -DF_CPU=$(SPEED) -ffixed-r2 -g
OUTDIR=build
PRJNAME=timertest
BASENAME = $(OUTDIR)/$(PRJNAME)
TARGET  = $(BASENAME).elf

.PHONY: $(TARGET)

$(TARGET): *.[cS]
	$(CC) $(CFLAGS) $^ -o $@
	avr-size $@

$(BASENAME).hex: $(BASENAME).elf
	objcopy -O ihex $^ $@

upload: $(BASENAME).hex
	avrdude -c $(PRG) -p $(MCU) -P $(PRGPORT) -U flash:w:$(BASENAME).hex:i

disasm: $(TARGET)
	avr-objdump -g -l -S -d $^ | less

clean:
	rm -f $(OUTDIR)/*

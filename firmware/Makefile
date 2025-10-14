MCU     ?= attiny20
SPEED   ?= 1000000L
CC      = avr-gcc
CFLAGS = -Wall -Wextra -Os -flto -mmcu=$(MCU) -DF_CPU=$(SPEED) -ffixed-r2 -g
OUTDIR=build
PRJNAME=prrplsattiny20.X.production
BASENAME = $(OUTDIR)/$(PRJNAME)
TARGET  = $(BASENAME).elf
TARGET_HEX  = $(BASENAME).hex

.PHONY: $(TARGET)

$(TARGET): src/*.[cS]
	$(CC) $(CFLAGS) $^ -o $@
	avr-objcopy -O ihex $@ $(TARGET_HEX)
	avr-size $@

disasm: $(TARGET)
	avr-objdump -g -l -S -d $^ | less

clean:
	rm -rf $(OUTDIR)/*

upload:
	./upload.py $(TARGET_HEX)

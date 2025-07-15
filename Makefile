MCU     ?= attiny20
SPEED   ?= 8000000L
CC      = avr-gcc
CFLAGS = -Os -flto -mmcu=$(MCU) -DF_CPU=$(SPEED) -ffixed-r2 -g
OUTDIR=dist/default/production
PRJNAME=prrplsattiny20.X.production
TARGET  = $(OUTDIR)/$(PRJNAME).elf

.PHONY: $(TARGET)

$(TARGET): src/*.[cS]
	$(CC) $(CFLAGS) $^ -o $@
	avr-size $@

disasm: $(TARGET)
	avr-objdump -g -l -S -d $^ | less

clean:
	rm -f $(OUTDIR)/*

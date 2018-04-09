# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.  This file is offered as-is,
# without any warranty.


MCU=		atmega32u4
DPROG=		avr109
DPORT=		/dev/ttyACM0

ISP_TARGET=	8031.ihx

TARGETS=	$(ISP_TARGET)
OBJECTS=	mcs51rom.o 8031.o

MCS51ROM=	mcs51code/hello.ihx

CC=		avr-gcc

CFLAGS=		-mmcu=$(MCU) -Os -std=c99
LDFLAGS=	-mmcu=$(MCU)

OBJCOPY=	avr-objcopy
OBJCOPYFLAGS=	-R .eeprom

AVRDUDE=	avrdude
AVRDUDEFLAGS=	-p $(MCU) -c $(DPROG) -P $(DPORT)


all:		$(TARGETS)


8031.elf:	$(OBJECTS)

8031.o:		mcs51rom.h

$(MCS51ROM):
		$(error missing $(MCS51ROM))

mcs51rom.bin:	$(MCS51ROM)
		srec_cat -Disable_Sequence_Warnings $< \
		    -Intel -Fill 0 0 0x1800 -Output $@ -Binary


%.elf : %.o
		$(CC) $(LDFLAGS) -o $@ $^

%.ihx : %.elf
		$(OBJCOPY) $(OBJCOPYFLAGS) -O ihex $< $@

isp:		$(ISP_TARGET)
		$(AVRDUDE) $(AVRDUDEFLAGS) -U flash:w:$(ISP_TARGET):i

clean:
		rm -f $(TARGETS) *.elf *.o mcs51rom.bin mcs51rom.h


# http://www.nongnu.org/avr-libc/user-manual/FAQ.html#faq_binarydata

%.o %.h : %.txt
	@echo Converting $< to $(*).o
	@cp $(<) $(*).tmp
	@echo -n 0 | tr 0 '\000' >> $(*).tmp
	@$(OBJCOPY) -I binary -O elf32-avr \
	--rename-section .data=.progmem.data,contents,alloc,load,readonly,data \
	--redefine-sym _binary_$(subst /,_,$*)_tmp_start=$(notdir $*) \
	--redefine-sym _binary_$(subst /,_,$*)_tmp_end=$(notdir $*)_end \
	--redefine-sym _binary_$(subst /,_,$*)_tmp_size=$(notdir $*)_size_sym \
	$(*).tmp $(*).o
	@echo "extern const char" $(notdir $*)"[] PROGMEM;" > $(*).h
	@echo "extern const char" $(notdir $*)_end"[] PROGMEM;" >> $(*).h
	@echo "extern const char" $(notdir $*)_size_sym"[];" >> $(*).h
	@echo "#define $(notdir $*)_size ((int)$(notdir *)_size_sym)" >> $(*).h
	@rm $(*).tmp

%.o %.h : %.bin
	@echo Converting $< to $(*).o
	@$(OBJCOPY) -I binary -O elf32-avr \
	--rename-section .data=.progmem.data,contents,alloc,load,readonly,data \
	--redefine-sym _binary_$(subst /,_,$*)_bin_start=$(notdir $*) \
	--redefine-sym _binary_$(subst /,_,$*)_bin_end=$(notdir $*)_end \
	--redefine-sym _binary_$(subst /,_,$*)_bin_size=$(notdir $*)_size_sym \
	$(<) $(*).o
	@echo "extern const char" $(notdir $*)"[] PROGMEM;" > $(*).h
	@echo "extern const char" $(notdir $*)_end"[] PROGMEM;" >> $(*).h
	@echo "extern const char" $(notdir $(*))_size_sym"[];" >> $(*).h
	@echo "#define $(notdir $*)_size ((int)$(notdir $*)_size_sym)" >> $(*).h

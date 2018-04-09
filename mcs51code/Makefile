# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.  This file is offered as-is,
# without any warranty.


TARGETS=	hello.ihx
HELLO_OBJECTS=	hello.rel

AS=		sdcc-sdas8051

CC=		sdcc-sdcc

CFLAGS=		-mmcs51
LDFLAGS=	-mmcs51
LDLIBS=		


all:		$(TARGETS)

hello.ihx:	$(HELLO_OBJECTS)

clean:
		$(RM) $(TARGETS) *.rel *.asm *.lst *.map *.mem *.noi *.rst *.sym *.lk

%.ihx : %.rel
		$(CC) $(LDFLAGS) $(LOADLIBES) $(LDLIBS) $^ -o $@

%.rel : %.s
		$(AS) $(ASFLAGS) -los $@ $<

%.rel : %.c
		$(CC) $(CFLAGS) -c $<

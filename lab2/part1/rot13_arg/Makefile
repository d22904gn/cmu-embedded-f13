CC      = gcc
OBJCOPY = objcopy
CFLAGS  = -O2 -ffreestanding -Wall -Werror -nostdinc -I ../libc/include
LDFLAGS = -nostdlib -Wl,-Ttext,$(LOAD_ADDR) 

LOAD_ADDR = 0xa2000000

CRT0    = ../libc/crt0.o
LDLIBS  = ../libc/libc.a

objects = rot13_arg.o

default: rot13_arg.bin

.PHONY: default clean clobber

rot13_arg.bin: rot13_arg
	$(OBJCOPY) -O binary $< $@

rot13_arg: $(objects)
	$(CC) $(LDFLAGS) -o $@ $(CRT0) $^ $(LDLIBS)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -f rot13 $(objects)

clobber: clean
	rm -f rot13.bin

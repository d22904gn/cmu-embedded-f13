CC      = gcc
CFLAGS  = -g -O2 -Wall -Werror
ASFLAGS = -g

COMMON_OBJS = main.o
STOCK_OBJS = strTable.o
OPTIMIZED_OBJS = strTable_optimized.o

default: part2asm optimized

.PHONY: default clean clobber

part2asm: $(COMMON_OBJS) $(STOCK_OBJS)
	$(CC) -g -o $@ $^

optimized: $(COMMON_OBJS) $(OPTIMIZED_OBJS)
	$(CC) -g -o $@ $^

main.o: main.c
strTable.o: strTable.s
strTable_optimized.o: strTable_optimized.s

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

%.o: %.s
	$(CC) -c $(ASFLAGS) -o $@ $<

clean:
	rm -f $(COMMON_OBJS) $(STOCK_OBJS) $(OPTIMIZED_OBJS)

clobber: clean
	rm -f part2asm

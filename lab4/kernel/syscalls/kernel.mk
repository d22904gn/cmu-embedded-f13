SYSCALLS_OBJS := swi_handler.o swi_handler_c.o io.o timing.o 
SYSCALLS_OBJS := $(SYSCALLS_OBJS:%=$(KDIR)/syscalls/%)

KOBJS += $(SYSCALLS_OBJS)

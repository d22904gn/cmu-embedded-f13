SYSCALLS_OBJS := swi_handler.o swi_handler_c.o io.o timing.o tasking.o \
				 sleep_tasking.o
SYSCALLS_OBJS := $(SYSCALLS_OBJS:%=$(KDIR)/syscalls/%)

KOBJS += $(SYSCALLS_OBJS)

INTERRUPTS_OBJS := irq_handler.o irq_handler_c.o timers.o
INTERRUPTS_OBJS := $(INTERRUPTS_OBJS:%=$(KDIR)/interrupts/%)

KOBJS += $(INTERRUPTS_OBJS)

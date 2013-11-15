INTERRUPT_OBJS := irq_handler.o irq_handler_c.o timers.o
INTERRUPT_OBJS := $(INTERRUPT_OBJS:%=$(KDIR)/interrupts/%)

KOBJS += $(INTERRUPT_OBJS)

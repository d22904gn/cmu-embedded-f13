SCHEDULER_OBJS := scheduler.o runqueue.o context.o context_asm.o \
				  devices.o mutex.o tcbqueue.o
SCHEDULER_OBJS := $(SCHEDULER_OBJS:%=$(KDIR)/scheduler/%)

KOBJS += $(SCHEDULER_OBJS)

SCHEDULER_OBJS := scheduler.o run_queue.o context_asm.o
SCHEDULER_OBJS := $(SCHEDULER_OBJS:%=$(KDIR)/scheduler/%)

KOBJS += $(SCHEDULER_OBJS)

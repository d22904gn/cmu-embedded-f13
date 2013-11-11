KERNEL = $(KDIR)/kernel
KSTART = $(KDIR)/start.o

# All core kernel objects go here.  Add objects here if you need to.
KOBJS := syscalls/swi_handler.o syscalls/swi_handler_c.o \
		 syscalls/exit.o syscalls/read.o syscalls/write.o \
		 syscalls/time.o syscalls/sleep.o \
		 interrupts/irq_handler.o interrupts/irq_handler_c.o \
		 interrupts/timers.o \
		 interrupts/enable_interrupts.o \
		 interrupts/disable_interrupts.o \
		 uboot_globals.o setup_irq_mode.o setup_usermode.o \
		 backup_uboot.o assert.o main.o raise.o
KOBJS := $(KOBJS:%=$(KDIR)/%)
ALL_OBJS += $(KOBJS) $(KSTART)
ALL_CLOBBERS += $(KERNEL) $(KERNEL).bin

-include $(KDIR)/arm/kernel.mk

# Put everything needed by the kernel into the final binary.
# KOBJS contains core kernel objects.
# AOBJS contains objects that are ARM dependent.
# UOBJS contains objects that are U-boot dependent.

$(KERNEL): $(KSTART) $(KOBJS) $(AOBJS) $(UOBJS)
	@echo LD $(notdir $@)
	@$(LD) -static $(LDFLAGS) -o $@ $^ $(LIBGCC)


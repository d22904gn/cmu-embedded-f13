KERNEL = $(KDIR)/kernel
KSTART = $(KDIR)/start.o

# All core kernel objects go here.  Add objects here if you need to.
KOBJS := swi_handler.o swi_handler_c.o exit.o syscalls.o setup_usermode.o assert.o main.o raise.o
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


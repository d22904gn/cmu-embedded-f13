UTILS_OBJS := math.o
UTILS_OBJS := $(UTILS_OBJS:%=$(KDIR)/utils/%)

KOBJS += $(UTILS_OBJS)

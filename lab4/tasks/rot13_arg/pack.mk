PROGS_ROT13_ARG_OBJS := rot13_arg.o
PROGS_ROT13_ARG_OBJS := $(PROGS_ROT13_ARG_OBJS:%=$(TDIR)/rot13_arg/%)
ALL_OBJS += $(PROGS_ROT13_ARG_OBJS)

$(TDIR)/bin/rot13_arg : $(TSTART) $(PROGS_ROT13_ARG_OBJS) $(TLIBC)


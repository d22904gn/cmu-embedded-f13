PROGS_TASK_NEST_OBJS := task_nest.o
PROGS_TASK_NEST_OBJS := $(PROGS_TASK_NEST_OBJS:%=$(TDIR)/task_nest/%)
ALL_OBJS += $(PROGS_TASK_NEST_OBJS)

$(TDIR)/bin/task_nest : $(TSTART) $(PROGS_TASK_NEST_OBJS) $(TLIBC)

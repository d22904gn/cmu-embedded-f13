PROGS_MUTEX_TEST_DEV_OBJS := mutex_test_dev.o
PROGS_MUTEX_TEST_DEV_OBJS := $(PROGS_MUTEX_TEST_DEV_OBJS:%=$(TDIR)/mutex_test_dev/%)
ALL_OBJS += $(PROGS_MUTEX_TEST_DEV_OBJS)

$(TDIR)/bin/mutex_test_dev : $(TSTART) $(PROGS_MUTEX_TEST_DEV_OBJS) $(TLIBC)

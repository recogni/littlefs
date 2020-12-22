#
# Build lfs by cd'ing into it and creating a liblfs.a
# that the rest of scorpio can link with.  This keeps 
# CFLAGS, headers and the like seperate
#

TOPDIR:=$(dir $(realpath $(lastword $(MAKEFILE_LIST))))/../..

include $(TOPDIR)/Makefile.rules

TARGET := liblfs.a

SRC := $(wildcard *.c)
SRC += bd/lfs_rambd.c
SRC += bd/lfs_sdhcibd.c

OBJS := $(SRC:.c=.o)

CFLAGS := -fomit-frame-pointer -fno-strict-aliasing -fno-builtin  -mstrict-align -mcmodel=medany -g -ggdb 
CFLAGS += -DLFS_NO_DEBUG -DLFS_NO_ERROR -DGRPC_COMPAT
CFLAGS += -I. $(SDHCI_INCLUDES) $(FREERTOS_INCLUDES) -I$(TOPDIR)/src/common/include -I$(TOPDIR)/src/scpu/include -I$(TOPDIR)/src/scpu
CFLAGS += -std=c99 -Wall -pedantic
CFLAGS += -Wextra -Wshadow -Wjump-misses-init -Wundef

all: $(OBJS) $(TARGET)

%.a: $(OBJS)
	$(AR) rcs $@ $^

%.o: %.c
	$(GCC) -c $(CFLAGS) -o $@ $<

.PHONY: clean1
clean1:
	rm -f $(TARGET) $(OBJS)

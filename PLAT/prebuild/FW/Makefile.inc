CFLAGS_INC += -I $(LIBDIR)/FW/inc
ifeq ($(BUILD_FW), y)
ifeq ($(TOOLCHAIN),GCC)
PREBUILDLIBS += $(LIBDIR)/FW/lib/gcc/libfw.a
endif

ifeq ($(TOOLCHAIN),ARMCC)
PREBUILDLIBS += $(LIBDIR)/FW/lib/keil/libfw.lib
endif
endif

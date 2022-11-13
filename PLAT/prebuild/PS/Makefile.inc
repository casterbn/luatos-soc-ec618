CFLAGS_INC += -I $(LIBDIR)/PS/inc

ifeq ($(BUILD_PS), y)
ifeq ($(TOOLCHAIN),GCC)

ifeq ($(LITE), false)
PS_PREBUILD_LIBS_DIR   ?= $(LIBDIR)/PS/lib/gcc
else
PS_PREBUILD_LIBS_DIR   ?= $(LIBDIR)/PS/lib/gcc/lite
endif

PREBUILDLIBS += $(PS_PREBUILD_LIBS_DIR)/libps.a   \
                $(PS_PREBUILD_LIBS_DIR)/libpsl1.a \
                $(PS_PREBUILD_LIBS_DIR)/libpsif.a 
endif

ifeq ($(TOOLCHAIN),ARMCC)

ifeq ($(LITE), false)
PS_PREBUILD_LIBS_DIR   ?= $(LIBDIR)/PS/lib/keil
else
PS_PREBUILD_LIBS_DIR   ?= $(LIBDIR)/PS/lib/keil/lite
endif

PREBUILDLIBS += $(PS_PREBUILD_LIBS_DIR)/libps.lib   \
                $(PS_PREBUILD_LIBS_DIR)/libpsl1.lib \
                $(PS_PREBUILD_LIBS_DIR)/libpsif.lib 
endif
endif
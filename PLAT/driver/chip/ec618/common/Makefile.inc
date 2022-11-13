
CFLAGS_INC += -I$(TOP)/PLAT/driver/chip/$(CHIP)/common/inc      

ifeq ($(TOOLCHAIN),GCC)
libdriver-y                               += PLAT/driver/chip/$(CHIP)/common/gcc/memcpy-armv7m.o
DRIVER_EXCLUDE_PPFILES                    := PLAT/driver/chip/$(CHIP)/common/gcc/memcpy-armv7m.pp
endif

libdriverprivate-$(DRIVER_IPC_ENABLE)     += PLAT/driver/chip/$(CHIP)/common/src/ipcdrvcomm.o
libdriverprivate-$(DRIVER_IPC_ENABLE)     += PLAT/driver/chip/$(CHIP)/common/src/sipcdrv.o
libdriverprivate-y                        += PLAT/driver/chip/$(CHIP)/common/src/aonreg.o \
                                             PLAT/driver/chip/$(CHIP)/common/src/cpmureg.o \
                                             PLAT/driver/chip/$(CHIP)/common/src/shareinfo.o
libdriverprivate-$(DRIVER_RESET_ENABLE)   += PLAT/driver/chip/$(CHIP)/common/src/reset.o 
libdriverprivate-$(DRIVER_PMU_ENABLE)     += PLAT/driver/chip/$(CHIP)/common/src/TmuDriver.o





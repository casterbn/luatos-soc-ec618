CFLAGS_INC += -I $(LIBDIR)/PLAT/inc

ifeq ($(TOOLCHAIN),ARMCC)

ifeq ($(LITE), false)
PLAT_PREBUILD_LIBS_DIR   ?= $(LIBDIR)/PLAT/lib/keil
else
PLAT_PREBUILD_LIBS_DIR   ?= $(LIBDIR)/PLAT/lib/keil/lite
endif

ifeq ($(BUILD_USE_PREBUILD_LIB),y)
PREBUILDLIBS += $(PLAT_PREBUILD_LIBS_DIR)/libosa.lib \
                $(PLAT_PREBUILD_LIBS_DIR)/libmiddleware_ec_private.lib \
                $(PLAT_PREBUILD_LIBS_DIR)/libccio.lib \
                $(PLAT_PREBUILD_LIBS_DIR)/libdeltapatch.lib \
                $(PLAT_PREBUILD_LIBS_DIR)/libfota.lib

ifeq ($(BINNAME),ap_bootloader)	
PREBUILDLIBS += $(PLAT_PREBUILD_LIBS_DIR)/libdriver_private_bl.lib
else
PREBUILDLIBS += $(PLAT_PREBUILD_LIBS_DIR)/libdriver_private.lib
endif
endif

ifeq ($(BUILD_USE_USB_LIB),y)
PREBUILDLIBS += $(PLAT_PREBUILD_LIBS_DIR)/libusb_private.lib
endif

endif

ifeq ($(TOOLCHAIN),GCC)

ifeq ($(LITE), false)
PLAT_PREBUILD_LIBS_DIR   ?= $(LIBDIR)/PLAT/lib/gcc
else
PLAT_PREBUILD_LIBS_DIR   ?= $(LIBDIR)/PLAT/lib/gcc/lite
endif

ifeq ($(BUILD_USE_PREBUILD_LIB),y)
PREBUILDLIBS += $(PLAT_PREBUILD_LIBS_DIR)/libosa.a \
                $(PLAT_PREBUILD_LIBS_DIR)/libmiddleware_ec_private.a \
                $(PLAT_PREBUILD_LIBS_DIR)/libccio.a \
                $(PLAT_PREBUILD_LIBS_DIR)/libdeltapatch.a \
                $(PLAT_PREBUILD_LIBS_DIR)/libfota.a

ifeq ($(BINNAME),ap_bootloader)
PREBUILDLIBS += $(PLAT_PREBUILD_LIBS_DIR)/libdriver_private_bl.a 
else ifeq ($(BINNAME),ap_driver_example)
PREBUILDLIBS += $(PLAT_PREBUILD_LIBS_DIR)/libdriver_private_drv.a
else
PREBUILDLIBS += $(PLAT_PREBUILD_LIBS_DIR)/libdriver_private.a
endif
endif

#qocm also use usb libs and bl/app use different libs
ifeq ($(BUILD_USE_USB_LIB),y)
ifeq ($(BINNAME),ap_bootloader)
PREBUILDLIBS += $(PLAT_PREBUILD_LIBS_DIR)/libusbbl_priv.a
else
ifeq ($(DRIVER_USB_ENABLE),y)
PREBUILDLIBS += $(PLAT_PREBUILD_LIBS_DIR)/libusb_private.a
endif
endif
endif

endif
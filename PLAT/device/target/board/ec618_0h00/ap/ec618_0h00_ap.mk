CHIP                        = ec618
CHIP_VERSION                = A0
CORE                        = ap
FPGA_DEBUG                  ?= n
OS                          = freertos
EUTRAN_CAT_MODE             ?= n
BUILD_USE_PREBUILD_LIB      = y
BUILD_USE_USB_LIB           = y
BUILD_FOR_KEY_CUSTOMER1     = n
BUILD_PS                    ?= y
BUILD_FW                    ?= n
#No mater BUILD_USE_PREBUILD_LIB, below is configurable.
DRIVER_DMA_ENABLE           ?= y
DRIVER_CACHE_ENABLE         ?= y
DRIVER_PMU_ENABLE           ?= y
DRIVER_FLASHRT_ENABLE       ?= y
DRIVER_FLASH_ENABLE         ?= y
DRIVER_CAMERA_ENABLE        ?= n
DRIVER_LCD_ENABLE           ?= n
DRIVER_GPR_ENABLE           ?= y
DRIVER_IC_ENABLE            ?= y
DRIVER_PWRKEY_ENABLE        ?= y
DRIVER_CHARGE_ENABLE        ?= y
DRIVER_ALARM_ENABLE         ?= y
DRIVER_CIPHER_ENABLE        ?= y
DRIVER_EFUSE_ENABLE         ?= y
DRIVER_SCT_ENABLE           ?= y
DRIVER_RNG_ENABLE           ?= y
DRIVER_MCUMODE_ENABLE       ?= y
DRIVER_ECMAIN_ENABLE        ?= y
DRIVER_PLATCFG_ENABLE       ?= y
RF_ADI_USED_ENABLE          ?= n
BUILD_EC_MW                 ?= y
BUILD_OS                    ?= y
BUILD_OSA                   ?= y
BUILD_AT                    ?= y
BUILD_AT_DEBUG              ?= y
#Choose if below driver need to be compiled or not
DRIVER_USART_ENABLE         ?= y
DRIVER_LPUART_ENABLE        ?= y
DRIVER_SPI_ENABLE           ?= y
DRIVER_I2C_ENABLE           ?= y
DRIVER_I2S_ENABLE           ?= y
DRIVER_GPIO_ENABLE          ?= y
DRIVER_TIMER_ENABLE         ?= y
DRIVER_ONEWIRE_ENABLE       ?= y
DRIVER_EEPROM_ENABLE        ?= n
DRIVER_TIMER_ENABLE         ?= y
DRIVER_UNILOG_ENABLE        ?= y
DRIVER_WDG_ENABLE           ?= y
DRIVER_ADC_ENABLE           ?= y
DRIVER_IPC_ENABLE           ?= y
DRIVER_RESET_ENABLE         ?= y
DRIVER_KPC_ENABLE           ?= n
DRIVER_HAL_ADC_ENABLE       ?= n
DRIVER_ULDP_ENABLE          ?= y
DRIVER_UTFC_ENABLE          ?= y
DRIVER_USB_ENABLE           ?= y
DRIVER_EXCP_ENABLE          ?= y
DRIVER_RFCALI_ENABLE        ?= y
DRIVER_PHYHAL_ENABLE        ?= y
DRIVER_CPADC_ENABLE         ?= y
DRIVER_NTC_ENABLE           ?= y
ifeq ($(DRIVER_EFUSE_ENABLE)_$(DRIVER_ADC_ENABLE),y_y)  
DRIVER_HAL_ADC_ENABLE = y
endif
#Choose if below thirdparty sofware need to be compiled
THIRDPARTY_MQTT_ENABLE      ?= n
THIRDPARTY_HTTPC_ENABLE     ?= n
THIRDPARTY_LITTEFS_ENABLE   ?= y
THIRDPARTY_IPERF_ENABLE     ?= y
THIRDPARTY_CJSON_ENABLE     ?= n
THIRDPARTY_LWIP_ENABLE      ?= y
THIRDPARTY_PING_ENABLE      ?= y
THIRDPARTY_DHCPD_ENABLE     ?= y
THIRDPARTY_MBEDTLS_ENABLE   ?= y
THIRDPARTY_LIBSNTP_ENABLE   ?= y
THIRDPARTY_CTCC_DM_ENABLE   ?= n
THIRDPARTY_CUCC_DM_ENABLE   ?= n
THIRDPARTY_CMCC_DM_ENABLE   ?= n
THIRDPARTY_EXAMPLE_ENABLE   ?= n
THIRDPARTY_MP3DECODE_ENABLE   ?= n
THIRDPARTY_MP3ENCODE_ENABLE   ?= n
THIRDPARTY_AACDECODE_ENABLE   ?= n
THIRDPARTY_AACENCODE_ENABLE   ?= n
THIRDPARTY_YRCOMPRESS_ENABLE ?= n
THIRDPARTY_DHRYSTONE_ENABLE ?= n
THIRDPARTY_COREMARK_ENABLE  ?= n
THIRDPARTY_CTWING_CERTI_ENABLE ?= n
#Choose if below ecomm middleware  need to be compiled
MIDDLEWARE_DEBUG_TRACE_ENABLE ?=n
MIDDLEWARE_CCIO_ENABLE        ?=y
MIDDLEWARE_CMS_ENABLE         ?=y
MIDDLEWARE_SIMBIP_ENABLE      ?=y
MIDDLEWARE_NVRAM_ENABLE       ?=y
MIDDLEWARE_FOTA_ENABLE        ?=n
MIDDLEWARE_FOTAPAR_ENABLE     ?=n
MIDDLEWARE_FEATURE_PLATTEST_ENABLE ?=n
#add for AT REF,default is undefined 
BUILD_AT_REF                  ?=n
LOW_SPEED_SERVICE_ONLY_ENABLE ?=n
LITE_FEATURE_MODE_ENABLE      ?=n
#default AT config value, should align with normal at cmd project  
include $(TOP)/PLAT/device/target/board/ec618_0h00/ap/at_config.inc
CFLAGS += -D__EC618
ifeq ($(CHIP_VERSION),A0)
CFLAGS += -DCHIP_EC618
ASMFLAGS += --predefine "CHIP_EC618_A0 SETL {TRUE} "
endif
#CFLAGS += --no_integer_literal_pools
ifeq ($(CORE),ap)
CFLAGS += -DCORE_IS_AP
endif
ifeq ($(LOW_SPEED_SERVICE_ONLY_ENABLE),y)
CFLAGS_DEFS += -DLOW_SPEED_SERVICE_ONLY
endif
ifeq ($(LITE_FEATURE_MODE_ENABLE),y)
CFLAGS_DEFS += -DLITE_FEATURE_MODE
else
CFLAGS += -fno-inline
CFLAGS += -mslow-flash-data
endif

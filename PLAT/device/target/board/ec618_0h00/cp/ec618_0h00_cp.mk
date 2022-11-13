CHIP                        = ec618
CHIP_VERSION                = A0

OS                          = freertos
BUILD_USE_PREBUILD_LIB      = n
BUILD_PS                    ?= n
BUILD_FW                    = y

RF_ADI_USED_ENABLE          ?= n
FPGA_DEBUG                   = n


#No mater BUILD_USE_PREBUILD_LIB, below is configurable.
DRIVER_DMA_ENABLE           ?= y
DRIVER_CACHE_ENABLE         ?= y
DRIVER_PMU_ENABLE           ?= y
DRIVER_FLASHRT_ENABLE       ?= y
DRIVER_FLASH_ENABLE         ?= y
DRIVER_GPR_ENABLE           ?= y
DRIVER_IC_ENABLE            ?= y
DRIVER_CIPHER_ENABLE        ?= n
DRIVER_CIPHER_FLH_ENABLE    ?= n
DRIVER_EFUSE_ENABLE         ?= y
DRIVER_CIPHER_TLS_ENABLE    ?= n
DRIVER_RNG_ENABLE           ?= y
DRIVER_MCUMODE_ENABLE       ?= n
DRIVER_IPC_ENABLE           ?= y
DRIVER_RESET_ENABLE         ?= y
DRIVER_HAL_ADC_ENABLE       ?= n
DRIVER_HAL_UART_ENABLE      ?= n
BUILD_OS                    ?= y
BUILD_OSA                   ?= n

#Choose if below driver need to be compiled or not
DRIVER_USART_ENABLE         ?= y
DRIVER_SPI_ENABLE           ?= y
DRIVER_GPIO_ENABLE          ?= y
DRIVER_TIMER_ENABLE         ?= y
DRIVER_UNILOG_ENABLE        ?= y
DRIVER_WDG_ENABLE           ?= y
DRIVER_ADC_ENABLE           ?= y
DRIVER_CLK_ENABLE           ?= y
DRIVER_EXCP_ENABLE          ?= y

MIDDLEWARE_NVRAM_ENABLE     ?=y

CFLAGS += -D__EC618
ifeq ($(CHIP_VERSION),A0)
CFLAGS += -DCHIP_EC618
ASMFLAGS += --predefine "CHIP_EC618_A0 SETL {TRUE} "
endif

ifeq ($(CORE),cp)
CFLAGS += -DCORE_IS_CP
endif

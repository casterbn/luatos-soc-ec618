##################################################################################################################
##Description：
##global option for PLAT AT commands
##Use method: y is support;n is not support
##default value is y to support
##################################################################################################################
ifeq ($(BUILD_AT), y)
BUILD_PLAT_CUST_AT_ENABLE    ?= y
BUILD_PLAT_MEM_AT_ENABLE     ?= y
BUILD_PLAT_PMU_AT_ENABLE     ?= y
BUILD_PLAT_PER_AT_ENABLE     ?= y
BUILD_PLAT_MISC_AT_ENABLE    ?= y
BUILD_PLAT_PROD_AT_ENABLE    ?= y

BUILD_PLAT_FOTA_AT_ENABLE    ?= y
BUILD_PLAT_HTTP_AT_ENABLE    ?= y
BUILD_PLAT_MQTT_AT_ENABLE    ?= y
BUILD_PLAT_SSL_AT_ENABLE     ?= y
endif

##################################################################################################################
##Description：BUILD_PLAT_CUST_AT_ENABLE
##global option for Plat product related AT commands
##Use method: y is support;n is not support
##default value is y to support
##################################################################################################################
ifeq ($(BUILD_PLAT_CUST_AT_ENABLE), y)
#AT+CGMI
BUILD_PLAT_CUST_CGMI_ENABLE   ?= y
#AT+CGMM
BUILD_PLAT_CUST_CGMM_ENABLE   ?= y
#AT+GMM
BUILD_PLAT_CUST_GMM_ENABLE    ?= y
#AT+CGMR
BUILD_PLAT_CUST_CGMR_ENABLE   ?= y
#AT+CGSN
BUILD_PLAT_CUST_CGSN_ENABLE   ?= y
#ATI
BUILD_PLAT_CUST_ATI_ENABLE    ?= y
#AT&W
BUILD_PLAT_CUST_ATnW_ENABLE   ?= y
#AT&F
BUILD_PLAT_CUST_ATnF_ENABLE   ?= y
#AT&V
BUILD_PLAT_CUST_ATnV_ENABLE   ?= y
endif

##################################################################################################################
##Description：BUILD_PLAT_MEM_AT_ENABLE
##global option for Plat memory related AT commands
##Use method: y is support;n is not support
##default value is y to support
##################################################################################################################
ifeq ($(BUILD_PLAT_MEM_AT_ENABLE), y)
#AT+ECSHOWMEM
BUILD_PLAT_MEM_ECSHOWMEM_ENABLE           ?= y
#AT+ECHEAPINFO
BUILD_PLAT_MEM_ECHEAPINFO_ENABLE          ?= y
#AT+ECMEM32
BUILD_PLAT_MEM_ECMEM32_ENABLE             ?= y
#AT+ECDLFCMEM
BUILD_PLAT_MEM_ECDLFCMEM_ENABLE           ?= y
#AT+ECFSINFO
BUILD_PLAT_MEM_ECFSINFO_ENABLE            ?= y
#AT+RLCHK
BUILD_PLAT_MEM_RLCHK_ENABLE               ?= y
#AT+ECFLASHMONITORINFO
BUILD_PLAT_MEM_ECFLASHMONITORINFO_ENABLE  ?= y
endif

##################################################################################################################
##Description：BUILD_PLAT_PMU_AT_ENABLE
##global option for Plat pmu related AT commands
##Use method: y is support;n is not support
##default value is y to support
##################################################################################################################
ifeq ($(BUILD_PLAT_PMU_AT_ENABLE), y)
#AT+ECPMUCFG
BUILD_PLAT_PMU_ECPMUCFG_ENABLE     ?= y
#AT+ECVOTECHK
BUILD_PLAT_PMU_ECVOTECHK_ENABLE    ?= y
#AT+ECPMUSTATUS
BUILD_PLAT_PMU_ECPMUSTATUS_ENABLE  ?= y
#AT+ECPURC
BUILD_PLAT_PMU_ECPURC_ENABLE       ?= y
#AT+ECSCLK
BUILD_PLAT_PMU_ECSCLK_ENABLE       ?= y
endif

##################################################################################################################
##Description：BUILD_PLAT_PER_AT_ENABLE
##global option for Plat peripheral related AT commands
##Use method: y is support;n is not support
##default value is y to support
##################################################################################################################
ifeq ($(BUILD_PLAT_PER_AT_ENABLE), y)
#AT+IPR
BUILD_PLAT_PER_IPR_ENABLE        ?= y
#AT+ICF
BUILD_PLAT_PER_ICF_ENABLE        ?= y
#AT+IFC
BUILD_PLAT_PER_IFC_ENABLE        ?= y
#AT+ECUSBSYS
BUILD_PLAT_PER_ECUSBSYS_ENABLE   ?= y
#AT+ECLEDMODE
BUILD_PLAT_PER_ECLEDMODE_ENABLE  ?= y
#AT+ECPALARM
BUILD_PLAT_PER_ECPALARM_ENABLE   ?= y
#AT+ECADC
BUILD_PLAT_PER_ECADC_ENABLE      ?= y
endif

##################################################################################################################
##Description：BUILD_PLAT_MISC_AT_ENABLE
##global option for Plat miscellaneous AT commands
##Use method: y is support;n is not support
##default value is y to support
##################################################################################################################
ifeq ($(BUILD_PLAT_MISC_AT_ENABLE), y)
#AT+ECPCFG
BUILD_PLAT_MISC_ECPCFG_ENABLE          ?= y
#AT+ECRST
BUILD_PLAT_MISC_ECRST_ENABLE           ?= y
#AT+ECPOWD
BUILD_PLAT_MISC_ECPOWD_ENABLE          ?= y
#AT^SYSTEST
BUILD_PLAT_MISC_SYSTEST_ENABLE         ?= y
#AT+ECSYSTEST
BUILD_PLAT_MISC_ECSYSTEST_ENABLE       ?= y
#AT+ECTASKINFO
BUILD_PLAT_MISC_ECTASKINFO_ENABLE      ?= y
#AT+ECTASKHISTINFO
BUILD_PLAT_MISC_ECTASKHISTINFO_ENABLE  ?= y
#AT+ECLOGDBVER
BUILD_PLAT_MISC_ECLOGDBVER_ENABLE      ?= y
#AT+ECDUMPCHK
BUILD_PLAT_MISC_ECDUMPCHK_ENABLE       ?= y
#AT+ECBTOFFSETDBG
BUILD_PLAT_MISC_ECBTOFFSETDBG_ENABLE   ?= y
endif

##################################################################################################################
##Description：BUILD_PLAT_PROD_AT_ENABLE
##global option for Plat product related AT commands
##Use method: y is support;n is not support
##default value is y to support
##################################################################################################################
ifeq ($(BUILD_PLAT_PROD_AT_ENABLE), y)
#AT+ECATE
BUILD_PLAT_PROD_ECATE_ENABLE       ?= y
#AT+ECVERSION
BUILD_PLAT_PROD_ECVERSION_ENABLE   ?= y
#AT+ECIPR
BUILD_PLAT_PROD_ECIPR_ENABLE       ?= y
#AT+ECSLEEP
BUILD_PLAT_PROD_ECSLEEP_ENABLE     ?= y
#AT+ECSAVEFAC
BUILD_PLAT_PROD_ECSAVEFAC_ENABLE   ?= y
#AT+ECRFTEST
BUILD_PLAT_PROD_ECRFTEST_ENABLE    ?= y
#AT+ECGMDATA
BUILD_PLAT_PROD_ECGMDATA_ENABLE    ?= y
#AT+ECNPICFG
BUILD_PLAT_PROD_ECNPICFG_ENABLE    ?= y
#AT+ECRFNST
BUILD_PLAT_PROD_ECRFNST_ENABLE     ?= y
#AT+ECRFSTAT
BUILD_PLAT_PROD_ECRFSTAT_ENABLE    ?= y
#AT+ECPRODMODE
BUILD_PLAT_PROD_ECPRODMODE_ENABLE  ?= n
endif

##################################################################################################################
##Description：
##global option for PS AT commands 
##Use method: y is support;n is not support
##default value is y to support 
##################################################################################################################
ifeq ($(BUILD_AT), y)
BUILD_PS_DEV_AT_ENABLE  ?= y
BUILD_PS_EMM_AT_ENABLE  ?= y
BUILD_PS_EPS_AT_ENABLE  ?= y
BUILD_PS_SIM_AT_ENABLE  ?= y
BUILD_PS_SMS_AT_ENABLE  ?= y
BUILD_PHY_CONFIG_AT_ENABLE ?= y
BUILD_PS_TCPIP_ECSOCK_AT_ENABLE  ?= y
BUILD_PS_TCPIP_SKTSOCK_AT_ENABLE ?= y
BUILD_PS_TCPIP_NETCONFIG_AT_ENABLE ?=y
endif
##################################################################################################################
##name :BUILD_PS_DEV_AT_ENABLE
##Description：
##BUILD_PS_DEV_AT_ENABLE is to control whether include ps  global AT commands as: AT+CFUN/ECBAND/ECCFG/ECSTATUS/...etc
##Use method: y is support;n is not support. 
##default value is y to support 
##################################################################################################################
ifeq ($(BUILD_PS_DEV_AT_ENABLE), y)

#AT/ATQ/ATE/ATT/ATV/ATL/ATM/ATS0/AT&C/AT&D/ATO/ATH/AT+CSCS/AT+CMEE/AT+ECURC/AT+ECURCCFG
BUILD_PS_DEV_COMMON_AT_ENABLE ?= y
#AT+CFUN
BUILD_PS_DEV_CFUN_ENABLE    ?= y
#AT+ECCGSN
BUILD_PS_DEV_ECCGSN_ENABLE  ?= y
#AT+ECCGSNLOCK
BUILD_PS_DEV_ECCGSNLOCK_ENABLE  ?= y
#AT+ECSTATUS
BUILD_PS_DEV_ECSTATUS_ENABLE  ?= y
#AT+ECSTATIS
BUILD_PS_DEV_ECSTATIS_ENABLE  ?= y
#AT+ECBAND
BUILD_PS_DEV_ECBAND_ENABLE    ?=  y
#AT+ECFREQ
BUILD_PS_DEV_ECFREQ_ENABLE    ?=  y
#AT+ECCFG
BUILD_PS_DEV_ECCFG_ENABLE     ?=  y
#AT+ECRMFPLMN
BUILD_PS_DEV_ECRMFPLMN_ENABLE ?=  y
#AT+ECBCINFO
BUILD_PS_DEV_ECBCINFO_ENABLE  ?=  y
#AT+ECPSTEST
BUILD_PS_DEV_ECPSTEST_ENABLE  ?=  y
#AT+ECEVENTSTATIS
BUILD_PS_DEV_ECEVENTSTATIS_ENABLE  ?=  y
#AT+ECNASTCFG
BUILD_PS_DEV_ECNASTCFG_ENABLE   ?=  y
#AT+ECWIFISCAN
BUILD_PS_DEV_ECWIFISCAN_ENABLE  ?=  y
endif
##################################################################################################################
##name :BUILD_PS_EMM_AT_ENABLE
##Description：
##BUILD_PS_EMM_AT_ENABLE is to control whether include ps  mm AT commands as: AT+CESQ/COPS/CCIOTOPT/ECCESQS/...etc
##Use method: y is support;n is not support
##default value is y to support 
##################################################################################################################
ifeq ($(BUILD_PS_EMM_AT_ENABLE), y)
#AT+CSQ
BUILD_PS_EMM_CSQ_ENABLE   ?=  y
#AT+CESQ
BUILD_PS_EMM_CESQ_ENABLE  ?=  y
#AT+CREG
BUILD_PS_EMM_CREG_ENABLE  ?=  y
#AT+COPS
BUILD_PS_EMM_COPS_ENABLE  ?=  y
#AT+CPSMS
BUILD_PS_EMM_CPSMS_ENABLE     ?= y
#AT+CEDRXS
BUILD_PS_EMM_CEDRXS_ENABLE    ?= y
#AT+CEDRXRDP
BUILD_PS_EMM_CEDRXRDP_ENABLE  ?= y
#AT+CCIOTOPT
BUILD_PS_EMM_CCIOTOPT_ENABLE  ?= y
#AT+CRCES
BUILD_PS_EMM_CRCES_ENABLE ?= y
#AT+CCLK
BUILD_PS_EMM_CCLK_ENABLE  ?= y
#AT+CTZR
BUILD_PS_EMM_CTZR_ENABLE  ?= y
#AT+CTZU
BUILD_PS_EMM_CTZU_ENABLE  ?= y
#AT+ECPLMNS
BUILD_PS_EMM_ECPLMNS_ENABLE     ?= y
#AT+ECCESQS
BUILD_PS_EMM_ECCESQS_ENABLE     ?= y
#AT+ECPSMR
BUILD_PS_EMM_ECPSMR_ENABLE      ?= y
#AT+ECPTWEDRXS
BUILD_PS_EMM_ECPTWEDRXS_ENABLE  ?= y
#AT+ECEMMTIME
BUILD_PS_EMM_ECEMMTIME_ENABLE   ?= y
#AT+ECPLMNRESELECT
BUILD_PS_EMM_ECPLMNRESELECT_ENABLE  ?= y
endif
##################################################################################################################
##name :BUILD_PS_EPS_AT_ENABLE
##Description：
##BUILD_PS_EPS_AT_ENABLE is to control whether include ESP  AT commands as: AT+CGATT/CGDCONT/CGACT/CGCONTRDP/CGAUTH..etc
##Use method: y is support;n is not support
##default value is y to support 
##################################################################################################################
ifeq ($(BUILD_PS_EPS_AT_ENABLE), y)
#AT+CGATT
BUILD_PS_EPS_CGATT_ENABLE  ?= y
#AT+CGDATA
BUILD_PS_EPS_CGDATA_ENABLE ?= y
#AT+CGTFT
BUILD_PS_EPS_CGTFT_ENABLE  ?= y
#AT+CGCMOD
BUILD_PS_EPS_CGCMOD_ENABLE ?= y
#AT+CGDCONT
BUILD_PS_EPS_CGDCONT_ENABLE ?= y
#AT+CGACT
BUILD_PS_EPS_CGACT_ENABLE   ?= y
#AT+CGEQOS
BUILD_PS_EPS_CGEQOS_ENABLE  ?= y
#AT+CGEQOSRDP
BUILD_PS_EPS_CGEQOSRDP_ENABLE ?= y
#AT+CGCONTRDP
BUILD_PS_EPS_CGCONTRDP_ENABLE ?= y
#AT+CGSCONTRDP
BUILD_PS_EPS_CGSCONTRDP_ENABLE ?= y
#AT+CGTFTRDP
BUILD_PS_EPS_CGTFTRDP_ENABLE   ?= y
#AT+CEREG
BUILD_PS_EPS_CEREG_ENABLE ?= y
#AT+CSCON
BUILD_PS_EPS_CSCON_ENABLE ?= y
#AT+CSODCP
BUILD_PS_EPS_CSODCP_ENABLE ?= y
#AT+CRTDCP
BUILD_PS_EPS_CRTDCP_ENABLE ?= y
#AT+CGAUTH
BUILD_PS_EPS_CGAUTH_ENABLE ?= y
#AT+CIPCA
BUILD_PS_EPS_CIPCA_ENABLE  ?= y
#AT+CGAPNRC
BUILD_PS_EPS_CGAPNRC_ENABLE ?= y
#AT+CGEREP
BUILD_PS_EPS_CGEREP_ENABLE  ?= y
#AT+CGDSCONT
BUILD_PS_EPS_CGDSCONT_ENABLE ?= y
#AT+CGPADDR
BUILD_PS_EPS_CGPADDR_ENABLE  ?= y
#AT+CEER
BUILD_PS_EPS_CEER_ENABLE   ?= y
#AT+CEMODE
BUILD_PS_EPS_CEMODE_ENABLE ?= y
#AT+CPSDO
BUILD_PS_EPS_CPSDO_ENABLE  ?= y
#AT+ECCIOTPLANE
BUILD_PS_EPS_ECCIOTPLANE_ENABLE ?= y
#AT+ECSENDDATA
BUILD_PS_EPS_ECSENDDATA_ENABLE  ?= y
#AT+ECGDCNT
BUILD_PS_EPS_ECGDCNT_ENABLE     ?= y
#AT+ECAUGDCNT
BUILD_PS_EPS_ECAUGDCNT_ENABLE   ?= y
#AT+ECSCLKEX
BUILD_PS_EPS_ECSCLKEX_ENABLE    ?= y
endif
##################################################################################################################
##name :BUILD_PS_TCPIP_ECSOCK_AT_ENABLE
##Description：
##BUILD_PS_TCPIP_ECSOCK_AT_ENABLE is to control whether include TCPIP EC SOCKET AT commands 
##Use method: y is support;n is not support
##default value is y to support 
##################################################################################################################
ifeq ($(BUILD_PS_TCPIP_ECSOCK_AT_ENABLE), y)
# do nothing here
endif
##################################################################################################################
##name :BUILD_PS_TCPIP_SKTSOCK_AT_ENABLE
##Description：
##BUILD_PS_TCPIP_ECSOCK_AT_ENABLE is to control whether include TCPIP SKT SOCKET AT commands 
##Use method: y is support;n is not support
##default value is y to support 
##################################################################################################################
ifeq ($(BUILD_PS_TCPIP_SKTSOCK_AT_ENABLE), y)
# do nothing here
endif
##################################################################################################################
##name :BUILD_AT_REF
##Description：
##BUILD_AT_REF is to control whether include REF AT commands
##Use method: y is support;n is not support
##default value is y to support 
##################################################################################################################
ifeq ($(BUILD_AT_REF), y)
#define QI sockets command      AT+QIOPEN/QICLOSE/QIRD/QISDE/QIGETERROR/QISTATE/QISEND/QISENDEX/QISWTMD
BUILD_PS_REF_TCPIP_AT_ENABLE      ?= y
#define QI socket command：     AT+QPING/QNTP
BUILD_PS_REF_TCPIP_SERVICE_AT_ENABLE   ?= y
#define QI socket command：    QIDNSCFG/QIDNSGIP/QNETDEVCTL
BUILD_PS_REF_TCPIP_CONFIG_AT_ENABLE   ?= y
#define QI socket command：    AT+QCCID/QENG/QCELL/QWIFISCAN/QPOWD/QSPN/QIACT/QIDEACT/QIREGAPP/QICSGP/QGDCNT/QAUGDCNT
BUILD_PS_REF_EPS_AT_ENABLE      ?= y
#define QI socket command：    AT+QICFG/QCFG
BUILD_PS_REF_COMMON_AT_ENABLE  ?=y
endif
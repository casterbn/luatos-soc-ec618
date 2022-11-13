/******************************************************************************

*(C) Copyright 2018 EIGENCOMM International Ltd.

* All Rights Reserved

******************************************************************************
*  Filename:
*
*  Description:
*
*  History:
*
*  Notes:
*
******************************************************************************/
#ifndef _ATEC_RF_H
#define _ATEC_RF_H

#include <stdint.h>
#include "at_util.h"

#include DEBUG_LOG_HEADER_FILE
#include "debug_trace.h"


/*********************************************************************************
* Defines
*********************************************************************************/
extern uint32_t ShareInfoAPSetPhyCaliMem(void);
#define  RF_NST_A2C_BUFF_ADDR                     ShareInfoAPSetPhyCaliMem()  // 0xC800 : 50K
#define  RF_NST_C2A_BUFF_ADDR                     (RF_NST_A2C_BUFF_ADDR+0xC800)
#define  RF_NST_TEMP_BUFF_ADDR1                   (RF_NST_C2A_BUFF_ADDR+0x3C00)
#define  RF_NST_TEMP_BUFF_ADDR2                   (RF_NST_TEMP_BUFF_ADDR1+0x7800)
#define  RF_NST_TABLE_BUFF_ADDR                   (RF_NST_TEMP_BUFF_ADDR2+0x3C00)


#define ATC_ECRFTEST_0_STR_MAX_LEN                8000         /* */
#define ATC_ECRFNST_0_STR_MAX_LEN                 8000         /* */

#define  RF_CALNST_PREHANDLE_CRC_ERROR            (-2)  // preHandle error
#define  RF_CALNST_TRANSDATABLOCK_INDEX_ERROR     (-3)

/*********************************************************************************
* Enums
*********************************************************************************/


/*********************************************************************************
* struct
*********************************************************************************/

CmsRetId phyECRFNST(const AtCmdInputContext *pAtCmdReq);
BOOL phyECRFOpenApiDcDcVpaCfg(UINT8 ena, UINT8 voltage);

#endif

/* END OF FILE */

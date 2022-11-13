/*
 * Copyright (c) 2022 OpenLuat & AirM2M
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "luat_pm.h"
#include "apmu_external.h"
#include "slpman.h"
#include "FreeRTOS.h"
#include "list.h"



static uint32_t reportMode[LUAT_PM_SLEEP_MODE_LIGHT + 1][10] = {0};

int luat_pm_set_sleep_mode(int mode, const char *vote_tag)
{
    if (mode < LUAT_PM_SLEEP_MODE_NONE || mode > LUAT_PM_SLEEP_MODE_LIGHT || vote_tag == NULL)
    {
        return -1;
    }

    bool findFirst = true;
    for (int sleepMode = LUAT_PM_SLEEP_MODE_NONE; sleepMode < LUAT_PM_SLEEP_MODE_LIGHT + 1; sleepMode++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (strcmp((const char *)reportMode[sleepMode][j], vote_tag) == 0)
            {
                findFirst = false;
                if (mode == sleepMode)
                {
                    break;
                }
                else
                {
                    luat_heap_free(reportMode[sleepMode][j]);
                    reportMode[sleepMode][j] = (uint32_t)NULL;
                    reportMode[sleepMode][9] = reportMode[sleepMode][9] - 1;
                    if (reportMode[mode][9] < 9)
                    {
                        reportMode[mode][j] = (uint32_t)luat_heap_malloc(strlen(vote_tag) + 1);
                        memset((uint32_t *)reportMode[mode][j], 0x00, strlen(vote_tag) + 1);
                        memcpy((uint32_t *)reportMode[mode][j], vote_tag, strlen(vote_tag) + 1);
                        reportMode[mode][9] = reportMode[mode][9] + 1;
                    }
                    break;
                }
            }
        }
    }
    if (findFirst)
    {
        if (reportMode[mode][9] < 9)
        {
            reportMode[mode][reportMode[mode][9]] = (uint32_t)luat_heap_malloc(strlen(vote_tag) + 1);
            memset((uint32_t *)reportMode[mode][reportMode[mode][9]], 0x00, strlen(vote_tag) + 1);
            memcpy((uint32_t *)reportMode[mode][reportMode[mode][9]], vote_tag, strlen(vote_tag) + 1);
            reportMode[mode][9] = reportMode[mode][9] + 1;
        }
    }
    for (int sleepMode = LUAT_PM_SLEEP_MODE_NONE; sleepMode < LUAT_PM_SLEEP_MODE_LIGHT + 1; sleepMode++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (reportMode[sleepMode][j] != (uint32_t)NULL)
            {
                switch (sleepMode)
                {
                case LUAT_PM_SLEEP_MODE_NONE:
                    apmuSetDeepestSleepMode(AP_STATE_ACTIVE);
                    break;
                case LUAT_PM_SLEEP_MODE_IDLE:
                    apmuSetDeepestSleepMode(AP_STATE_IDLE);
                    break;
                case LUAT_PM_SLEEP_MODE_LIGHT:
                    apmuSetDeepestSleepMode(AP_STATE_SLEEP1);
                    break;
                default:
                    apmuSetDeepestSleepMode(AP_STATE_IDLE);
                    break;
                }
                return 0;
            }
        }
    }
}

int luat_pm_get_sleep_mode(const char *vote_tag)
{
    for (int sleepMode = LUAT_PM_SLEEP_MODE_NONE; sleepMode < LUAT_PM_SLEEP_MODE_LIGHT + 1; sleepMode++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (strcmp((const char *)reportMode[sleepMode][j], vote_tag) == 0)
            {
                return sleepMode;
            }
        }
    }
    return -1;
}



/* typedef struct luat_pm_callback
{
    xLIST list;
    xLIST_ITEM callback[5];
}luat_pm_callback_t;
 */

static luat_pm_sleep_callback_t g_s_user_pre_callback = NULL;
static luat_pm_sleep_callback_t g_s_user_post_callback = NULL;

static slpManBackupCb_t sleep_pre_callback(void *param, slpManLpState mode)
{
    if (g_s_user_pre_callback != NULL)
    {
        g_s_user_pre_callback(mode);
    }
}

static slpManBackupCb_t sleep_post_callback(void *param, slpManLpState mode)
{
    if (g_s_user_post_callback != NULL)
    {
        g_s_user_post_callback(mode);
    }
}

int luat_pm_sleep_register_pre_handler(luat_pm_sleep_callback_t callback_fun)
{
    g_s_user_pre_callback = callback_fun;
    return slpManRegisterUsrdefinedBackupCb(sleep_pre_callback, NULL);
}
int luat_pm_sleep_deregister_pre_handler(void)
{
    g_s_user_pre_callback = NULL;
    return slpManUnregisterUsrdefinedBackupCb(sleep_pre_callback);
}
int luat_pm_sleep_register_post_handler(luat_pm_sleep_callback_t callback_fun)
{
    g_s_user_post_callback = callback_fun;
    return slpManRegisterUsrdefinedRestoreCb(sleep_post_callback, NULL);
}
int luat_pm_sleep_deregister_post_handler(void)
{
    g_s_user_post_callback = NULL;
    return slpManUnregisterUsrdefinedRestoreCb(sleep_post_callback);
}

int luat_pm_wakeup_pad_set_callback(luat_pm_wakeup_pad_isr_callback_t callback_fun)
{
    soc_set_pad_wakeup_callback(callback_fun);
    return 0;
}

int luat_pm_wakeup_pad_set(uint8_t enable, LUAT_PM_WAKEUP_PAD_E source_id, luat_pm_wakeup_pad_cfg_t *cfg)
{
    if ((enable != 1 && enable != 0) || ((source_id < LUAT_PMWAKEUP_PAD_0) && (source_id > LUAT_PMWAKEUP_PAD_5)) || cfg == NULL)
    {
        return -1;
    }
    APmuWakeupPadSettings_t padConfig = {0};
    padConfig.negEdgeEn = cfg->neg_edge_enable;
    padConfig.posEdgeEn = cfg->pos_edge_enable;
    padConfig.pullDownEn = cfg->pull_down_enable;
    padConfig.pullUpEn = cfg->pull_up_enable;
    apmuSetWakeupPadCfg(source_id, enable, &padConfig);
    if(enable)
        NVIC_EnableIRQ(source_id);
    else
        NVIC_DisableIRQ(source_id);
    return 0;
}
int luat_pm_get_poweron_reason(void)
{
    return 0;
}
int luat_pm_poweroff(void)
{
    return 0;
}
int luat_pm_reboot(void)
{
    return 0;
}
int luat_pm_get_battery_volt(uint16_t *volt)
{
    return 0;
}
int luat_pm_get_vbus_status(uint8_t *status)
{
    return 0;
}
int luat_pm_event_register_handler(luat_pm_event_callback_t callback_fun)
{
    return 0;
}
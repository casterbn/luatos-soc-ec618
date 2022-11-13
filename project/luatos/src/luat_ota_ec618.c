
#include "common_api.h"
#include "luat_base.h"
#include "luat_ota.h"

#include "mem_map.h"

int luat_ota_exec(void) {
    return luat_ota(FLASH_FOTA_REGION_START - 128 * 1024);
}

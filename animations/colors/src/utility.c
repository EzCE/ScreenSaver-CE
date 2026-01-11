#include "utility.h"

#include <fileioc.h>
#include <time.h>
#include <graphx.h>

#include <sys/power.h>

bool utility_ChkAPDTimer(void) {
    static clock_t offset = 0;
    static uint8_t apdTimer = 0;

    if (!offset) {
        offset = clock();
        uint8_t slot = ti_Open("ScrnSavr", "r");
        apdTimer = *(uint8_t *)ti_GetDataPtr(slot);
    } else if (apdTimer && clock() - offset >= apdTimer * ONE_MINUTE && !boot_BatteryCharging()) {
        return true;
    }

    return false;
}

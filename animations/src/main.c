#include "screensavers.h"

#include <graphx.h>
#include <keypadc.h>
#include <fileioc.h>

/**
 * Launch the currently selected screensaver animation.
 */
int main(void) {
    uint8_t slot = ti_Open("ScrnSavr", "r");
    uint8_t selected = *(uint8_t *)ti_GetDataPtr(slot);
    ti_Close(slot);

    static void (*animations[ANIMATION_COUNT]) (void) = {hello, toasters, pipes, fish, walk, aod, tiles}; // Add animation functions here
    animations[selected]();

    return 0;
}

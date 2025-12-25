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

    if (!slot || selected >= ANIMATION_COUNT) {
        return 1; // Turn off if AppVar is invalid
    }

    static bool (*animations[ANIMATION_COUNT]) (void) = {toasters, pipes, fish, walk, aod, tiles, spirograph, matrix, triss, merth}; // Add animation functions here

    return animations[selected]();
}

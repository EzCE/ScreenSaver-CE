#include "utility.h"

#include <graphx.h>
#include <keypadc.h>
#include <sys/lcd.h>
#include <sys/util.h>
#include <sys/rtc.h>

int main(void) {
    uint8_t brightnessBackup = lcd_BacklightLevel;
    unsigned int delayCounter = 0;
    bool increment = true;
    srand(rtc_Time());
    gfx_Begin();
    lcd_BacklightLevel = 255;
    gfx_ZeroScreen();

    while (!kb_AnyKey()) {              // Main animation loop
        if (utility_ChkAPDTimer()) {
            gfx_End();
            return 1;                   // Check to power off
        }

        delayCounter++;

        if (delayCounter % 20 == 0) {
            if (increment) {
                lcd_BacklightLevel += 2;
            } else {
                lcd_BacklightLevel -= 2;
            }
            delayCounter = 0;
        }

        if (lcd_BacklightLevel == 255) {
            *lcd_Palette = randInt(0, 0xFFFF);
            increment = false;
        }

        if (lcd_BacklightLevel == 1) {
            increment = true;
        }
    }

    lcd_BacklightLevel = brightnessBackup;

    gfx_End();

    return 0;                           // Exit without powering off
}

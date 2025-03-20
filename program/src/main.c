#include "asm/utils.h"
#include "../../appvar/src/screensavers.h"

#include <fileioc.h>
#include <graphx.h>
#include <keypadc.h>
#include <time.h>
#include <ti/getcsc.h>
#include <ti/screen.h>

void redraw(bool option, bool hookEnabled, uint8_t animation) {
    static const char *names[ANIMATION_COUNT] = {"Hello"}; // Add screensaver names here

    gfx_SetColor(189);

    if (!option) {
        gfx_SetColor(255);
    }

    gfx_Rectangle_NoClip(204, 62, 41, 11);
    gfx_SetColor(189);

    if (option) {
        gfx_SetColor(255);
    }

    gfx_Rectangle_NoClip(164, 87, 81, 11);
    gfx_SetColor(42);
    gfx_FillRectangle_NoClip(205, 63, 39, 9);
    gfx_FillRectangle_NoClip(165, 88, 79, 9);
    gfx_SetTextXY(206, 64);
    gfx_PrintChar('<');
    gfx_SetTextXY(166, 89);
    gfx_PrintChar('<');
    gfx_SetTextXY(238, 64);
    gfx_PrintChar('>');
    gfx_SetTextXY(238, 89);
    gfx_PrintChar('>');

    if (hookEnabled) {
        gfx_PrintStringXY("Yes", 213, 64);
    } else {
        gfx_PrintStringXY("No", 217, 64);
    }

    gfx_PrintStringXY(names[animation], 204 - gfx_GetStringWidth(names[animation]) / 2, 89);
}

int main(void) {
    uint8_t slot = ti_Open("ScrnSavr", "r");

    if (!slot) {
        ti_Close(slot);
        os_ClrHomeFull();
        os_PutStrLine("ScrnSavr AppVar not found.");
        while (!os_GetCSC());
        return 0;
    }

    bool option = false;
    bool hookEnabled = isHookInstalled();
    uint8_t animation = *(uint8_t *)ti_GetDataPtr(slot);

    gfx_Begin();
    gfx_SetDrawBuffer();
    gfx_FillScreen(10);
    gfx_SetTextBGColor(10);
    gfx_SetTextFGColor(189);
    gfx_SetTextScale(2, 2);
    gfx_PrintStringXY("ScreenSaver", 73, 14);
    gfx_PrintStringXY("Options", 109, 31);
    gfx_SetTextScale(1, 1);
    gfx_PrintStringXY("Enabled:", 75, 64);
    gfx_PrintStringXY("Animation:", 75, 89);
    gfx_SetColor(189);
    gfx_HorizLine_NoClip(73, 54, 174);
    gfx_HorizLine_NoClip(73, 105, 174);
    gfx_SetTextBGColor(42);
    redraw(option, hookEnabled, animation);
    gfx_BlitBuffer();

    bool keyPressed = false;

    while (kb_AnyKey());

    clock_t clockOffset = clock();

    while (!kb_IsDown(kb_KeyClear)) {
        if (!kb_AnyKey() && keyPressed) {
            keyPressed = false;
            clockOffset = clock();
        }

        if (kb_Data[7] && (!keyPressed || clock() - clockOffset > CLOCKS_PER_SEC / 32)) {
            clockOffset = clock();

            if (kb_IsDown(kb_KeyUp) || kb_IsDown(kb_KeyDown)) {
                option = !option;
            }

            if (!option) { // Toggle screensaver hook on / off
                if (kb_IsDown(kb_KeyLeft) || kb_IsDown(kb_KeyRight)) {
                    hookEnabled = !hookEnabled;
                }
            } else { // Select screensaver animation from list
                if (kb_IsDown(kb_KeyLeft)) {
                    if (animation) {
                        animation--;
                    } else {
                        animation = ANIMATION_COUNT - 1;
                    }
                } else if (kb_IsDown(kb_KeyRight)) {
                    animation++;

                    if (animation == ANIMATION_COUNT) {
                        animation = 0;
                    }
                }
            }

            redraw(option, hookEnabled, animation);
            gfx_BlitBuffer();

            if (!keyPressed) {
                while (clock() - clockOffset < CLOCKS_PER_SEC / 2.25 && kb_AnyKey()) {
                    kb_Scan();
                }
            }

            keyPressed = true;
        }

        kb_Scan();
    }

    if (hookEnabled) {
        installHook();
    } else {
        asm(
            "ld iy,$D00080\n\t" // ld iy, ti.flags
            "call $0213E4" // call ti.ClrGetKeyHook
        );
    }

    gfx_End();

    if (ti_IsArchived(slot)) {
        ti_SetArchiveStatus(false, slot);
    }

    *(uint8_t *)ti_GetDataPtr(slot) = animation;
    ti_SetArchiveStatus(true, slot);
    ti_Close(slot);

    return 0;
}

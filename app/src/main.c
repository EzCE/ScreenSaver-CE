#include "asm/utils.h"
#include "../../animations/src/screensavers.h"

#include <fileioc.h>
#include <graphx.h>
#include <keypadc.h>
#include <time.h>
#include <ti/getcsc.h>
#include <ti/screen.h>

// void noPreview(void) {
//     gfx_SetColor(42);
//     gfx_FillRectangle_NoClip(96, 131, 128, 96);
//     gfx_PrintStringXY("No preview", 124, 173);
// }

void drawButtonBG(unsigned int x, uint8_t y, uint8_t width, bool selected) {
    if (selected) {
        gfx_SetColor(189);
        gfx_FillRectangle_NoClip(x - 3, y - 2, width + 6, 11);
        gfx_HorizLine_NoClip(x - 2, y - 3, width + 4);
        gfx_HorizLine_NoClip(x - 2, y + 9, width + 4);
        gfx_SetTextFGColor(10);
    } else {
        gfx_SetTextFGColor(189);
        gfx_SetColor(10);
        gfx_FillRectangle_NoClip(x - 3, y - 3, width + 6, 13);
    }

    gfx_SetTextXY(x, y);
    gfx_PrintChar('<');
    gfx_SetTextXY(x + width - 5, y);
    gfx_PrintChar('>');
}

void redraw(uint8_t option, bool hookEnabled, uint8_t animation, uint8_t apdTimer) {
    static const char *names[ANIMATION_COUNT] = {
        "Toasters", 
        "Pipes", 
        "Fish", 
        "Walk",
        "Strands",
        "Clock", 
        "Tiles", 
        "Spiral", 
        "Matrix", 
        "Triss", 
        "Beziers",
        "Snow",
        "Bubbles", 
        "Merth"
    }; // Add screensaver names here
    
    static const char *authors[ANIMATION_COUNT] = {
        "TIny, RoccoLox", 
        "TIny, RoccoLox", 
        "Merth", 
        "Merth",
        "Merth",
        "TIny, RoccoLox", 
        "TIny, RoccoLox", 
        "Merth", 
        "Merth, v3x3d", 
        "Merth", 
        "Merth",
        "Merth",
        "Merth",
        "Merth"
    }; // Add screensaver authors here
    
    // static void (*previews[ANIMATION_COUNT]) (void) = {}; // Add screensaver previews here (WIP)

    // previews[animation]();
    // gfx_SetTextBGColor(10);
    // gfx_SetTextScale(1, 1);

    gfx_SetColor(189);

    if (!option) {
        gfx_SetColor(255);
    }

    gfx_Rectangle_NoClip(204, 62, 41, 11);
    gfx_SetColor(189);
    // gfx_Rectangle_NoClip(96, 131, 128, 96);

    drawButtonBG(206, 64, 37, option == 0);

    if (hookEnabled) {
        gfx_PrintStringXY("Yes", 213, 64);
    } else {
        gfx_PrintStringXY("No", 217, 64);
    }

    drawButtonBG(189, 82, 53, option == 1);

    if (!apdTimer) {
        gfx_PrintStringXY("Never", 196, 82);
    } else {
        if (apdTimer < 10) {
            gfx_SetTextXY(200, 82);
        } else {
            gfx_SetTextXY(196, 82);
        }

        gfx_PrintUInt(apdTimer, 1);
        gfx_PrintString(" min");
    }

    drawButtonBG(166, 100, 77, option == 2);
    gfx_PrintStringXY(names[animation], 204 - gfx_GetStringWidth(names[animation]) / 2, 100);
    gfx_SetColor(10);
    gfx_FillRectangle_NoClip(126, 126, 119, 8);
    gfx_SetTextFGColor(189);
    gfx_PrintStringXY(authors[animation], 245 - gfx_GetStringWidth(authors[animation]), 126);
}

int main(void) {
    gfx_SetTextBGColor(0);
    gfx_SetTextTransparentColor(0);
    uint8_t slot = ti_Open("ScrnSavr", "r");
    uint8_t animation = 0;
    uint8_t apdTimer = 5;

    if (slot) {
        ti_Read(&animation, sizeof(animation), 1, slot);
        ti_Seek(sizeof(animation), SEEK_SET, slot);
        ti_Read(&apdTimer, sizeof(apdTimer), 1, slot);
    }

    ti_Close(slot);

    uint8_t option = 0;
    bool hookEnabled = isHookInstalled();

    gfx_Begin();
    gfx_SetDrawBuffer();
    gfx_FillScreen(10);
    gfx_SetTextFGColor(189);
    gfx_SetTextScale(2, 2);
    gfx_PrintStringXY("ScreenSaver", 73, 14);
    gfx_PrintStringXY("Options", 109, 31);
    gfx_SetTextScale(1, 1);
    gfx_PrintStringXY("Enabled:", 75, 64);
    gfx_PrintStringXY("Off after:", 75, 82);
    gfx_PrintStringXY("Animation:", 75, 100);
    gfx_PrintStringXY("Author:", 75, 126);
    gfx_SetColor(189);
    gfx_HorizLine_NoClip(73, 54, 174);
    gfx_HorizLine_NoClip(73, 116, 174);
    redraw(option, hookEnabled, animation, apdTimer);
    gfx_BlitBuffer();

    bool keyPressed = false;

    while (kb_AnyKey());

    clock_t clockOffset = clock();

    while (!kb_IsDown(kb_KeyClear)) {
        if (!kb_AnyKey() && keyPressed) {
            keyPressed = false;
            clockOffset = clock();
        }

        if (kb_Data[7] && (!keyPressed || clock() - clockOffset > CLOCKS_PER_SEC / 16)) {
            clockOffset = clock();

            if (kb_IsDown(kb_KeyUp)) {
                if (!option) {
                    option = 2;
                } else {
                    option--;
                }
            } else if (kb_IsDown(kb_KeyDown)) {
                if (option == 2) {
                    option = 0;
                } else {
                    option++;
                }
            }

            switch (option) {
                case 0: // Toggle screensaver hook on / off
                    if (kb_IsDown(kb_KeyLeft) || kb_IsDown(kb_KeyRight)) {
                        hookEnabled = !hookEnabled;
                    }

                    break;
                case 1:
                    if (kb_IsDown(kb_KeyLeft)) {
                        if (!apdTimer) {
                            apdTimer = 45;
                        } else {
                            apdTimer -= 5;
                        }
                    } else if (kb_IsDown(kb_KeyRight)) {
                        if (apdTimer == 45) {
                            apdTimer = 0;
                        } else {
                            apdTimer += 5;
                        }
                    }

                    break;
                case 2: // Select screensaver animation from list
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

                    break;
            }

            redraw(option, hookEnabled, animation, apdTimer);
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

    gfx_End();

    slot = ti_Open("ScrnSavr", "w");
    ti_Write(&animation, sizeof(animation), 1, slot);
    ti_Seek(sizeof(animation), SEEK_SET, slot);
    ti_Write(&apdTimer, sizeof(apdTimer), 1, slot);
    ti_SetArchiveStatus(true, slot);
    ti_Close(slot);

    if (hookEnabled) {
        installHook();
    } else {
        asm(
            "ld iy,$D00080\n\t" // ld iy, ti.flags
            "call $0213E4" // call ti.ClrGetKeyHook
        );
    }

    return 0;
}

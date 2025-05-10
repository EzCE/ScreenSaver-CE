#include <graphx.h>
#include <keypadc.h>

void hello(void) {
    gfx_Begin();
    gfx_ZeroScreen();

    gfx_SetTextTransparentColor(0);
    gfx_SetTextBGColor(0);
    gfx_SetTextFGColor(255);
    gfx_PrintString("Hello world!");

    while (!kb_AnyKey());

    gfx_End();
}

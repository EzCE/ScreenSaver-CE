#include <graphx.h>
#include <keypadc.h>

int main(void) {
    gfx_Begin();
    gfx_ZeroScreen();
    while (!kb_AnyKey()) {
        kb_Scan();
    }
    /*asm(
        "push hl\n\t"
        "ld hl, -1\n\t"
        "ld (hl), 2\n\t"
        "pop hl"
    );*/
    gfx_End();
    return 0;
}

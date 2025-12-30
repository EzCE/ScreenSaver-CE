#include "utility.h"

#include <keypadc.h>

int main(void) {
    while (!kb_AnyKey()) {              // Main animation loop
        if (utility_ChkAPDTimer()) {
            return 1;                   // Check to power off
        }
    }

    return 0;                           // Exit without powering off
}

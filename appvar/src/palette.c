#include "palette.h"

uint16_t HsvToRgb(uint8_t h, uint8_t s, uint8_t v)
{
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;

    if (s == 0)
    {
        r = v;
        g = v;
        b = v;
        return gfx_RGBTo1555(r, g, b);
    }

    uint8_t region = h / 43;
    uint8_t remainder = (h - (region * 43)) * 6; 

    uint8_t p = (v * (255 - s)) >> 8;
    uint8_t q = (v * (255 - ((s * remainder) >> 8))) >> 8;
    uint8_t t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

    switch (region)
    {
        case 0:
            r = v; g = t; b = p;
            break;
        case 1:
            r = q; g = v; b = p;
            break;
        case 2:
            r = p; g = v; b = t;
            break;
        case 3:
            r = p; g = q; b = v;
            break;
        case 4:
            r = t; g = p; b = v;
            break;
        default:
            r = v; g = p; b = q;
            break;
    }

    return gfx_RGBTo1555(r, g, b);
}

void palette_shift(uint16_t* palette)
{
    palette[255] = palette[1];
    for (int i = 1; i < 255; i++)
        palette[i] = palette[i+1];
}

void palette_rainbow(uint16_t* palette)
{
    for (uint16_t i = 1; i < 256; i++)
        palette[i] = HsvToRgb(i - 1, 255, 255);
}
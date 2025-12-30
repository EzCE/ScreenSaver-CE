#include <fileioc.h>
#include <graphx.h>
#include <keypadc.h>
#include <time.h>

#include <sys/rtc.h>
#include <sys/util.h>

#include "utility.h"

typedef enum FishDirection { fd_Right, fd_Left } FishDirection;

typedef struct Fish {
    FishDirection   Direction;
    uint8_t         Color;
    uint8_t         Width;
    uint8_t         Height;
    float           X;
    float           Y;
    float           Speed;
} Fish;

#define MAX_FISH 15
#define DEFAULT_BACKGROUND_COLOR 0

#define MAX_WIDTH 20
#define MIN_WIDTH 4

#define MAX_HEIGHT 20
#define MIN_HEIGHT 4

#define MARGIN 5

#define MAX_SPEED 4.0f
#define MIN_SPEED 0.5f

#define MAX_SPOOKED_SPEED 9.0f

#define SPOOK_CHANCE 666

#define BIG_FISH_WIDTH  50
#define BIG_FISH_HEIGHT (GFX_LCD_HEIGHT / 2 - 4)

static bool fish_shouldRemove(Fish* fish)
{
    return fish->X < -(fish->Width + MARGIN)
        || fish->X > (fish->Width + MARGIN + GFX_LCD_WIDTH);
}

#define randPercent() (randInt(0, 100)/100.0f)

static void fish_generate(Fish* fish) 
{
    fish->Direction = random() % 2;
    fish->Color = randInt(0, 255);

    if (!randPercent()) 
    {
        fish->Width = randInt(MAX_WIDTH, BIG_FISH_WIDTH);
        fish->Height = randInt(MAX_HEIGHT*2, BIG_FISH_HEIGHT);
        
        fish->Y = GFX_LCD_HEIGHT / 2;
        fish->X = fish->Direction == fd_Right ? -MARGIN : (GFX_LCD_WIDTH + fish->Width + MARGIN);
        fish->Speed = randPercent()*MAX_SPEED*3 + MIN_SPEED;
    } else 
    {
        fish->Width = randInt(MIN_WIDTH, MAX_WIDTH);
        fish->Height = randInt(MIN_HEIGHT, MAX_HEIGHT);
        
        fish->Y = randInt(fish->Height * 2 + 1, GFX_LCD_HEIGHT - fish->Height * 2 - 1);
        fish->X = fish->Direction == fd_Right ? -MARGIN : (GFX_LCD_WIDTH + fish->Width + MARGIN);
        fish->Speed = randPercent()*MAX_SPEED + MIN_SPEED;
    }
}

static bool fish_step(Fish* fishes, const uint8_t maxFish)
{
    for (uint8_t i = 0; i < maxFish; i += 1)
    {
        Fish* fish = &fishes[i];
        if (!randInt(0, SPOOK_CHANCE))
        {
            fish->Direction = !fish->Direction;
            fish->Speed *= 2.0f;

            if (fish->Speed > MAX_SPOOKED_SPEED)
                fish->Speed = MAX_SPOOKED_SPEED;
        }

        switch (fish->Direction) {
            case fd_Left:
                fish->X -= fish->Speed;
                break;
            case fd_Right:
                fish->X += fish->Speed;
                break;
                break;
        }

        if (fish_shouldRemove(fish))
            fish_generate(fish);
    }

    return !kb_AnyKey();
}

#define TAIL_MARGIN 3
#define EYE_MARGIN 4

static void fish_drawBody(const Fish* fish)
{
    gfx_SetColor(fish->Color);
    gfx_FillEllipse(fish->X, fish->Y, fish->Width, fish->Height);
}

static void fish_drawEye(const Fish* fish)
{
    uint24_t eyeX;
    uint8_t eyeY;

    switch (fish->Direction) {
        case fd_Left:
            eyeX = fish->X - fish->Width + fish->Width / 4;
            eyeY = fish->Y - EYE_MARGIN;
            break;
        default:
        case fd_Right:
            eyeX = fish->X + fish->Width - fish->Width / 4;
            eyeY = fish->Y - EYE_MARGIN;
            break;
    }

    gfx_SetColor(255);
    gfx_SetPixel(eyeX - 1, eyeY);
    gfx_SetPixel(eyeX + 1, eyeY);
    gfx_SetPixel(eyeX, eyeY - 1);
    gfx_SetPixel(eyeX, eyeY + 1);
    gfx_SetColor(0);
    gfx_SetPixel(eyeX, eyeY);
}

static void fish_drawMouth(const Fish* fish)
{
    uint24_t mounthX;

    switch (fish->Direction) {
        case fd_Left:
            mounthX = fish->X - fish->Width;
            break;
        default:
        case fd_Right:
            mounthX = fish->X + fish->Width - fish->Width / 4;
            break;
    }

    gfx_SetColor(0);
    gfx_HorizLine(mounthX, fish->Y, fish->Width / 4);
}

static void fish_drawTail(const Fish* fish)
{
    float x1, x2, x3;
    float y1, y2, y3;

    switch (fish->Direction)
    {
        case fd_Left:
            x1 = fish->X + fish->Width - TAIL_MARGIN;
            y1 = fish->Y;

            x2 = fish->X + fish->Width + fish->Width / 4;
            y2 = fish->Y - fish->Height / 2;


            x3 = fish->X + fish->Width + fish->Width / 4;
            y3 = fish->Y + fish->Height / 2;
        break;
        default:
        case fd_Right:
            x1 = fish->X - fish->Width + TAIL_MARGIN;
            y1 = fish->Y;
            
            x2 = fish->X - fish->Width - fish->Width / 4;
            y2 = fish->Y - fish->Height / 2;
            
            x3 = fish->X - fish->Width - fish->Width / 4;
            y3 = fish->Y + fish->Height / 2;
        break;
    }

    gfx_SetColor(fish->Color);
    gfx_FillTriangle(x1, y1, x2, y2, x3, y3);
}

static void fish_draw(const Fish* fishes, const uint8_t maxFish, const uint8_t backgroundColor)
{
    gfx_FillScreen(backgroundColor);
    for (uint8_t i = 0; i < maxFish; i += 1)
    {
        const Fish* fish = &fishes[i];
        fish_drawBody(fish);
        fish_drawEye(fish);
        fish_drawMouth(fish);
        fish_drawTail(fish);
    }
}

#define APPVAR_NAME             "[FISH"
#define HEADER_LENGTH           5
#define VERSION_FIELD_LENGTH    1
#define NUM_FISH_FIELD_LENGTH   1
#define BG_COLOR_FIELD_LENGTH   1
#define VERSION_NUM             11

static bool strnequ(char* src, char* dst, uint8_t len)
{
    for (uint8_t i = 0; i < len; i++)
    {
        if (src[i] != dst[i])
            return false;
    }

    return true;
}

static uint8_t fish_read(Fish* fishes, uint8_t* numFishes, uint8_t* backgroundColor)
{
    uint8_t var = ti_Open(APPVAR_NAME, "r");
    if (var == 0)
    {
        ti_Close(var);
        return 255;
    }

    if (ti_GetSize(var) <= HEADER_LENGTH + VERSION_FIELD_LENGTH + BG_COLOR_FIELD_LENGTH + NUM_FISH_FIELD_LENGTH)
    {
        ti_Close(var);
        return 1;
    }

    char buffer[HEADER_LENGTH];
    if (ti_Read(buffer, sizeof(char), HEADER_LENGTH, var) != HEADER_LENGTH)
    {
        ti_Close(var);
        return 2;
    }

    if (!strnequ(buffer, APPVAR_NAME, HEADER_LENGTH))
    {
        ti_Close(var);
        return 3;
    }
    
    uint8_t versionRead;
    if (ti_Read(&versionRead, sizeof(uint8_t), VERSION_FIELD_LENGTH, var) != VERSION_FIELD_LENGTH)
    {
        ti_Close(var);
        return 4;
    }
    
    if (versionRead != VERSION_NUM)
    {
        ti_Close(var);
        return 5;
    }

    uint8_t backgroundColorRead;
    if (ti_Read(&backgroundColorRead, sizeof(uint8_t), BG_COLOR_FIELD_LENGTH, var) != BG_COLOR_FIELD_LENGTH)
    {
        ti_Close(var);
        return 6;
    }

    uint8_t numFishRead;
    if (ti_Read(&numFishRead, sizeof(uint8_t), NUM_FISH_FIELD_LENGTH, var) != NUM_FISH_FIELD_LENGTH)
    {
        ti_Close(var);
        return 7;
    }

    uint8_t actualAmountOfFishToRead = numFishRead <= (*numFishes) ? numFishRead : *numFishes;
    
    if (ti_Read(fishes, sizeof(Fish), numFishRead, var) != actualAmountOfFishToRead)
    {
        ti_Close(var);
        return 8;
    }
    
    *backgroundColor = backgroundColorRead;
    *numFishes = numFishRead;
    
    ti_SetArchiveStatus(true, var);
    ti_Close(var);
    return 0;
}

int main(void) 
{
    gfx_Begin();
    gfx_SetDrawBuffer();

    srand(rtc_Time());

    Fish fishes[MAX_FISH];
    uint8_t backgroundColor = DEFAULT_BACKGROUND_COLOR;

    uint8_t maxFish = MAX_FISH;
    uint8_t readResult = fish_read(fishes, &maxFish, &backgroundColor);
    if (readResult) 
    {
        maxFish = MAX_FISH;
        for (uint8_t i = 0; i < MAX_FISH; i++)
            fish_generate(&fishes[i]);
    }

    while (fish_step(fishes, maxFish))
    {
        if (utility_ChkAPDTimer()) {
            gfx_End();
            return true;
        }
        fish_draw(fishes, maxFish, backgroundColor);
        gfx_SwapDraw();
    }

    gfx_End();
    return 0;
}

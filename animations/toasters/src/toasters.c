#include "utility.h"

#include "gfx/gfx.h"

#include <graphx.h>
#include <keypadc.h>
#include <time.h>

#include <sys/rtc.h>
#include <sys/util.h>

struct object_t {
    int x;
    int y;
    uint8_t velocity;
    uint8_t frame;
    struct object_t *next;
    struct object_t *prev;
};

static struct object_t *new(struct object_t *parent) {
    struct object_t *new = malloc(sizeof(struct object_t));

    if (parent != NULL) {
        new->next = parent->next;
        parent->next->prev = new;
        parent->next = new;
        new->prev = parent;
    } else {
        new->next = new;
        new->prev = new;
    }

    if (randInt(0, 1)) {
        new->x = randInt(40, 280);
        new->y = -32;
    } else {
        new->x = 352;
        new->y = randInt(40, 200);
    }

    new->velocity = randInt(1, 2);
    new->frame = 0;

    return new;
}

static void delete(struct object_t *object) {
    object->prev->next = object->next;
    object->next->prev = object->prev;

    free(object);
}

static void draw(struct object_t *object, const gfx_sprite_t **frames) {
    gfx_TransparentSprite(frames[object->frame], object->x, object->y);

    object->x -= object->velocity;
    object->y += object->velocity;
}

int main(void) {
    gfx_Begin();
    gfx_SetPalette(palette_toaster, sizeof_palette_toaster, 0);
    gfx_SetTransparentColor(1);
    gfx_SetDrawBuffer();

    srand(rtc_Time());

    struct object_t *objToast = new(NULL);
    uint8_t toasts = 1;
    struct object_t *objToaster = new(NULL);
    uint8_t toasters = 1;

    static const gfx_sprite_t *toastFrames[1] = {toast};
    static const gfx_sprite_t *toasterFrames[3] = {toaster0, toaster1, toaster2};

    clock_t offset = clock();

    while (!kb_AnyKey()) {
        if (utility_ChkAPDTimer()) {
            gfx_End();
            return true;
        }

        gfx_ZeroScreen();

        bool frame = (clock() - offset > CLOCKS_PER_SEC / 8);

        for (uint8_t i = 0; i < toasts || i < toasters; i++) {
            draw(objToast, toastFrames);

            if (objToast->x < -32 || objToast->x > 352 || objToast->y < -32 || objToast->y > 272) {
                delete(objToast);
                toasts--;

                if (!toasts) {
                    objToast = new(NULL);
                    toasts = 1;
                }
            }

            objToast = objToast->next;
        }

        for (uint8_t i = 0; i < toasters; i++) {
            draw(objToaster, toasterFrames);

            if (frame) {
                if (objToaster->frame < 2) {
                    objToaster->frame += 1;
                } else {
                    objToaster->frame = 0;
                }
            }

            if (objToaster->x < -32 || objToaster->x > 352 || objToaster->y < -32 || objToaster->y > 272) {
                delete(objToaster);
                toasters--;

                if (!toasters) {
                    objToaster = new(NULL);
                    toasters = 1;
                }
            }

            objToaster = objToaster->next;
        }

        gfx_BlitBuffer();

        if (frame) {
            offset = clock();

            if (toasters + toasts < 16) {
                switch (randInt(0, 5)) {
                    case 0:
                        new(objToast);
                        toasts++;
                        break;
                    case 1:
                        new(objToaster);
                        toasters++;
                        break;
                    default:
                        break;
                }
            }
        }
    }

    gfx_End();

    return 0;
}

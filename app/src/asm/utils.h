#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void installHook(void);

bool isHookInstalled(void);

/**
 * @brief Sorts the VAT alphabetically.
 * 
 */
void sortVAT(void);

/**
 * @brief Preview an animation.
 * 
 * @param name Name of animation AppVar to preview.
 */
void previewAnimation(char *name);

#ifdef __cplusplus
}
#endif

#endif

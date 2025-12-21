/**
 * --------------------------------------
 * 
 * This file should include all
 * screensaver animation programs, along
 * with the total number of animations
 * available.
 * 
 * --------------------------------------
**/

#ifndef SCREENSAVERS_H
#define SCREENSAVERS_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Number of screensaver animations.
 */
#define ANIMATION_COUNT     9

/**
 * Placeholder hello world.
 */
bool hello(void);

/**
 * Flying toasters!
 */
bool toasters(void);

/**
 * Pipes.
 */
bool pipes(void);

/**
 * Fish.
 */
bool fish(void);

/**
 * Rainbow walk.
 */
bool walk(void);

/**
 * Always-on display clock.
 */
bool aod(void);

/**
 * Flipping tiles.
 */
bool tiles(void);

/**
 * Spirograph
 */
bool spirograph(void);

/**
 * Matrix rain.
 */
bool matrix(void);

#ifdef __cplusplus
}
#endif

#endif

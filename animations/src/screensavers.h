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
#define ANIMATION_COUNT     15

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

/**
 * Triss.
 */
bool triss(void);

/**
 * Bouncing Merth face.
 */
bool merth(void);

/**
 * Beziers.
 */
bool beziers(void);

/**
 * Falling snow.
 */
bool snow(void);

/**
 * Bouncing bubbles.
 */
bool bubbles(void);

/**
 * Rainbow strands.
 */
bool strands(void);

/**
 * Falling rain.
 */
bool rain(void);

#ifdef __cplusplus
}
#endif

#endif

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

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Number of screensaver animations.
 */
#define ANIMATION_COUNT     7

/**
 * Placeholder hello world.
 */
void hello(void);

/**
 * Flying toasters!
 */
void toasters(void);

/**
 * Pipes.
 */
void pipes(void);

/**
 * Fish.
 */
void fish(void);

/**
 * Rainbow walk.
 */
void walk(void);

/**
 * Always-on display clock.
 */
void aod(void);

/**
 * Flipping tiles.
 */
void tiles(void);

#ifdef __cplusplus
}
#endif

#endif

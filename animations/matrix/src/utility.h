#ifndef UTILITY_H
#define UTILITY_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * Timer defines for turning off the calculator after a set amount of
 * inactivity.
*/
#define ONE_SECOND      32768               /** One second. */
#define ONE_MINUTE      (ONE_SECOND * 60)   /** One minute. */

/**
 * @brief Stop displaying a screensaver and turn the calculator off after 
 *        a user-specified amount of time.
 * @return bool True if APD should be triggered.
 */
bool utility_ChkAPDTimer(void);

#ifdef __cplusplus
}
#endif

#endif

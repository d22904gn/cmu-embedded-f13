/**
 * @file timer.h
 *
 * @brief Definitions for the OS timer.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 *
 * @date 2008-07-07
 *
 * @note The addresses here are the addresses stated in the Intel PXA255
 *       Processor Developer's Manual minus 0x40000000.  This is so that
 *       this memory region can be relocated if we ever turn on the MMU.
 */

#ifndef _TIMER_H_
#define _TIMER_H_

#define OSTMR_OSMR_ADDR(x)    (0x00A00000 + (x)*4)    /* OS Timer Match Register */

#define OSTMR_OSCR_ADDR       0x00A00010   /* OS Timer Count Register */

#define OSTMR_OSSR_ADDR       0x00A00014   /* OS Timer Status Register */
#define OSTMR_OSSR_M0         0x00000001   /* Matched 0 */
#define OSTMR_OSSR_M1         0x00000002   /* Matched 1 */
#define OSTMR_OSSR_M2         0x00000004   /* Matched 2 */
#define OSTMR_OSSR_M3         0x00000008   /* Matched 3 */

#define OSTMR_OWER_ADDR       0x00A00018   /* OS Timer Watchdog Enable Register */
#define OSTMR_OWER_WME        0x00000001   /* Watchdog mode enable */

#define OSTMR_OIER_ADDR       0x00A0001C   /* OS Timer Interrupt Enable Register */
#define OSTMR_OIER_E0         0x00000001   /* Enable match 0 */
#define OSTMR_OIER_E1         0x00000002   /* Enable match 1 */
#define OSTMR_OIER_E2         0x00000004   /* Enable match 2 */
#define OSTMR_OIER_E3         0x00000008   /* Enable match 3 */

//#define OSTMR_FREQ            3686400      /* Oscillator frequency in hz */
#define OSTMR_FREQ            3250000      /* Oscillator frequency in hz */

/* Milliseconds per overflow */
#define MS_PER_OVERFLOW       ((UINT32_MAX / OSTMR_FREQ) * 1000u)

#ifndef ASSEMBLER

#include <types.h>
#include <inline.h>

// Convert a OSCR reading to milliseconds
INLINE unsigned long get_ms(uint32_t counter_val) {
    return counter_val / (OSTMR_FREQ / 1000);
}

// Convert milliseconds to num of OSCR ticks needed.
// Assumes millis <= <counter size> / <clock freq>
INLINE uint32_t get_ticks(uint32_t millis) {
    return (OSTMR_FREQ / 1000) * millis;
}

#endif /* ASSEMBLER */

#endif /* _TIMER_H_ */

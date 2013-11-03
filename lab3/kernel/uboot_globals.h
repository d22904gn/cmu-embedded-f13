/**
 * @file   uboot_globals.h
 *
 * @brief  Declares globals to store old U-Boot stuff in.
 *
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    02 Nov 2013
 */

#include <types.h>

#ifndef UBOOT_GLOBALS_H
#define UBOOT_GLOBALS_H

// Stores U-Boot stack pointer upon entry to main().
uint32_t uboot_sp;

/*
 * SWI Stuff
 */
// Stores address of the U-Boot SWI handler
uint32_t *uboot_swi_handler_addr;

// Stores old U-Boot SWI handler instructions.
uint32_t uboot_swi_instr1;
uint32_t uboot_swi_instr2;

/*
 * IRQ Stuff
 */
// Stores address of the U-Boot IRQ handler
uint32_t *uboot_irq_handler_addr;

// Stores old U-Boot IRQ handler instructions.
uint32_t uboot_irq_instr1;
uint32_t uboot_irq_instr2;

#endif

/**
 * @file    physmem.h
 *
 * @brief   Definitions for Intel PXA-255 physical memory layout.
 *          Also includes sanity checking routines.
 *
 * @author  Kartik Subramanian <ksubrama@andrew.cmu.edu>
 *
 * @date    2008-11-16
 * 
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    24 Nov 2013
 */

#ifndef _PHYSMEM_H_
#define _PHYSMEM_H_

#define FLASH_START_ADDR 0x00000000
#define FLASH_END_ADDR   0x01000000
#define RAM_START_ADDR   0xa0000000
#define RAM_END_ADDR     0xa4000000

#ifndef ASSEMBLER

#include <types.h>
#include <config.h>
#include <inline.h>

INLINE bool in_ram(uint32_t addr) {
    return (RAM_START_ADDR <= addr && addr <= RAM_END_ADDR);
}

INLINE bool in_readable_mem(uint32_t addr) {
    return (addr <= FLASH_END_ADDR) ||
           (RAM_START_ADDR <= addr && addr <= RAM_END_ADDR);
}

INLINE bool in_userspace(uint32_t addr) {
    return (USR_PROG_ENTRY <= addr && addr < USR_STACK_START);
}

#endif /* ASSEMBLER */

#endif /* _PHYSMEM_H_ */

/** @file config.h
 *
 * @brief A set of common OS configuration flags.  This is to control overall
 * OS behavior and not behavior of a particular modules.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-28
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

#define USR_PROG_ENTRY      0xa0000000
#define USR_STACK_START     0xa3000000

#define OS_KSTACK_SIZE      4096
#define OS_USTACK_ALIGN     1024

/* OS_MAX_TASKS must be atleast 8 and must be atmost 64 */
#define IDLE_PRIO           63
#define OS_MAX_TASKS        64
#define OS_AVAIL_TASKS      63

/* OS_NUM_MUTEX must be at least 32 */
#define OS_NUM_MUTEX        32

#endif /* _CONFIG_H_ */

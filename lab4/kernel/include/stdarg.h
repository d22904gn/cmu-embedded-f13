/*
 * stdarg.h: Defines variable argument macros & typedef
 *
 * Author: Mike Kasick <mkasick@andrew.cmu.edu>
 * Date:   Sun, 14 Oct 2007 00:12:03 -0400
 */

#ifndef _STDARG_H_
#define _STDARG_H_

#define va_start(ap, last) __builtin_va_start(ap, last)
#define va_arg(ap, type)   __builtin_va_arg(ap, type)
#define va_end(ap)         __builtin_va_end(ap)
#define va_copy(dest, src) __builtin_va_copy(dest, src)

typedef __builtin_va_list va_list;

#endif /* _STDARG_H_ */

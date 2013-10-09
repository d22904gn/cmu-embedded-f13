@ exit.S: exit sycall wrapper
@
@ Authors: Wee Loong Kuan <wkuan@andrew.cmu.edu>
@          Chin Yang Oh <chinyano@andrew.cmu.edu>
@          Jennifer Lee <jcl1@andrew.cmu.edu>
@ Date:    10 Sep 2013

#include <bits/swi.h>
	.file	"exit.S"
	.text

	.global	exit
exit:
	swi EXIT_SWI

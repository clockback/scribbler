#include <stdarg.h>

#include "WaitAction.h"

void WaitAction_init(void * me_void, va_list * args) {
	WaitActionPtr me = (WaitActionPtr)me_void;
	me->duration = va_arg(*args, int);
	me->current = 0;
}

bool WaitAction_run(void * me_void) {
	WaitActionPtr me = (WaitActionPtr)me_void;
	return (me->current ++) >= me->duration;
}

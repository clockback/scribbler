#include <stdarg.h>

#include "SetUserInputAction.h"

void SetUserInputAction_init(void * me_void, va_list * args) {
	SetUserInputActionPtr me = (SetUserInputActionPtr)me_void;
	me->system = va_arg(*args, SystemPtr);
	me->allow_user_input = (bool)va_arg(*args, int);
}

bool SetUserInputAction_run(void * me_void) {
	SetUserInputActionPtr me = (SetUserInputActionPtr)me_void;
	System_accept_user_input(me->system, me->allow_user_input);

	return true;
}

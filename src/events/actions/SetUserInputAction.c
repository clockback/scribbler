#include <stdarg.h>

#include "SetUserInputAction.h"

void SetUserInputAction_init(
	void * me_void, IoObject * io_particulars, IoHandler * io_handler,
	GamePtr game
) {
	SetUserInputActionPtr me = (SetUserInputActionPtr)me_void;

	me->system = game->sys;

	IoSymbol * get_allow_user_input = IoState_symbolWithCString_(
		io_handler->iostate, "allow_user_input"
	);
	IoObject * io_allow_user_input = IoObject_getSlot_(
		io_particulars, get_allow_user_input
	);

	if (IoNumber_asInt(io_allow_user_input)) {
		me->allow_user_input = true;
	}
	else {
		me->allow_user_input = false;
	}
}

bool SetUserInputAction_run(void * me_void) {
	SetUserInputActionPtr me = (SetUserInputActionPtr)me_void;
	System_accept_user_input(me->system, me->allow_user_input);

	return true;
}

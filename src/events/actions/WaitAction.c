#include <stdarg.h>

#include "WaitAction.h"

void WaitAction_init(
	void * me_void, IoObject * io_particulars, IoHandler * io_handler,
	GamePtr game
) {
	WaitActionPtr me = (WaitActionPtr)me_void;

	IoSymbol * get_duration = IoState_symbolWithCString_(
		io_handler->iostate, "duration"
	);

	IoObject * io_duration = IoObject_getSlot_(io_particulars, get_duration);

	me->duration = (NumericPtr)malloc(sizeof(Numeric));

	IoHandler_process(io_handler, io_duration, me->duration);

	me->current = 0;
}

bool WaitAction_run(void * me_void) {
	WaitActionPtr me = (WaitActionPtr)me_void;
	return (me->current ++) >= Numeric_evaluate(me->duration);
}

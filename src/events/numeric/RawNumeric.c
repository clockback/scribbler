#include <stdarg.h>

#include "RawNumeric.h"

void RawNumeric_init(
	void * me_void, IoObject * io_particulars, IoHandler * io_handler,
	GamePtr game
) {
	RawNumericPtr me = (RawNumericPtr)me_void;
	me->number = IoNumber_asDouble(io_particulars);
}

double RawNumeric_evaluate(void * me_void) {
	RawNumericPtr me = (RawNumericPtr)me_void;
	return me->number;
}

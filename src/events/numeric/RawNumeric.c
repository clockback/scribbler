#include <stdarg.h>

#include "RawNumeric.h"

void RawNumeric_init(void * me_void, va_list * args) {
	RawNumericPtr me = (RawNumericPtr)me_void;
	me->number = va_arg(*args, double);
}

double RawNumeric_evaluate(void * me_void) {
	RawNumericPtr me = (RawNumericPtr)me_void;
	return me->number;
}

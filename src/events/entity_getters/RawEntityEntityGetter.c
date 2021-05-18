#include <stdarg.h>

#include "RawEntityEntityGetter.h"

void RawEntityEntityGetter_init(void * me_void, va_list * args) {
	RawEntityEntityGetterPtr me = (RawEntityEntityGetterPtr)me_void;
	me->entity = va_arg(*args, EntityPtr);
}

EntityPtr RawEntityEntityGetter_evaluate(void * me_void) {
	RawEntityEntityGetterPtr me = (RawEntityEntityGetterPtr)me_void;
	return me->entity;
}

#include <stdarg.h>

#include "./InteractEntityTrigger.h"

void InteractEntityTrigger_init(void * me_void, va_list * args) {
	InteractEntityTriggerPtr me = (InteractEntityTriggerPtr)me_void;
	me->object = va_arg(*args, EntityPtr);
}

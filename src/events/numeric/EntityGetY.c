#include <stdarg.h>

#include "EntityGetY.h"

void EntityGetY_init(void * me_void, va_list * args) {
	EntityGetYPtr me = (EntityGetYPtr)me_void;
	me->entity = va_arg(*args, EntityGetterPtr);
}

double EntityGetY_evaluate(void * me_void) {
	EntityGetYPtr me = (EntityGetYPtr)me_void;
	MappedComponentPtr mapped = (MappedComponentPtr)Entity_fetch_component(
		EntityGetter_evaluate(me->entity), MAPPED_COMPONENT
	);
	return mapped->y;
}

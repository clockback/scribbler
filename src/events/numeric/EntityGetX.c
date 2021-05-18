#include <stdarg.h>

#include "EntityGetX.h"

void EntityGetX_init(void * me_void, va_list * args) {
	EntityGetXPtr me = (EntityGetXPtr)me_void;
	me->entity = va_arg(*args, EntityGetterPtr);
}

double EntityGetX_evaluate(void * me_void) {
	EntityGetXPtr me = (EntityGetXPtr)me_void;
	MappedComponentPtr mapped = (MappedComponentPtr)Entity_fetch_component(
		EntityGetter_evaluate(me->entity), MAPPED_COMPONENT
	);
	return mapped->x;
}

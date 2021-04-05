#include <stdarg.h>
#include <stdio.h>

#include "MappedComponent.h"

ComponentPtr MappedComponent_init(
	void * me_void, EntityPtr entity, va_list * args
) {
	MappedComponentPtr me = (MappedComponentPtr)me_void;
	ComponentPtr me_component = (ComponentPtr)me;
	Component_init(me_component, entity, MAPPED_COMPONENT);

	me->x = va_arg(*args, double);
	me->y = va_arg(*args, double);

	me->plane = va_arg(*args, PlanePtr);

	return me_component;
}

void MappedComponent_update(void * me_void) {
}

void MappedComponent_draw(void * me_void) {

}

void MappedComponent_delete(void * me_void) {

}

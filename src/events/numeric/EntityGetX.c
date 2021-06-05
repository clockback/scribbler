#include <stdarg.h>

#include "EntityGetX.h"

void EntityGetX_init(
	void * me_void, IoObject * io_particulars, IoHandler * io_handler,
	GamePtr game
) {
	EntityGetXPtr me = (EntityGetXPtr)me_void;

	IoSymbol * get_entity = IoState_symbolWithCString_(
		io_handler->iostate, "entity"
	);
	IoObject * io_entity = IoObject_getSlot_(io_particulars, get_entity);

	me->entity = (EntityGetterPtr)malloc(sizeof(EntityGetter));

	IoHandler_process(io_handler, io_entity, me->entity);
}

double EntityGetX_evaluate(void * me_void) {
	EntityGetXPtr me = (EntityGetXPtr)me_void;
	MappedComponentPtr mapped = (MappedComponentPtr)Entity_fetch_component(
		EntityGetter_evaluate(me->entity), MAPPED_COMPONENT
	);
	return mapped->x;
}

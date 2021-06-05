#include <stdarg.h>

#include "EntityJourneyToAction.h"

#include "../EntityGetter.h"

void EntityJourneyToAction_init(
	void * me_void, IoObject * io_particulars, IoHandler * io_handler,
	GamePtr game
) {
	EntityJourneyToActionPtr me = (EntityJourneyToActionPtr)me_void;

	IoSymbol * get_entity = IoState_symbolWithCString_(
		io_handler->iostate, "entity"
	);
	IoSymbol * get_x = IoState_symbolWithCString_(io_handler->iostate, "x");
	IoSymbol * get_y = IoState_symbolWithCString_(io_handler->iostate, "y");
	IoSymbol * get_plane = IoState_symbolWithCString_(
		io_handler->iostate, "plane"
	);

	IoObject * io_entity = IoObject_getSlot_(io_particulars, get_entity);
	IoObject * io_x = IoObject_getSlot_(io_particulars, get_x);
	IoObject * io_y = IoObject_getSlot_(io_particulars, get_y);
	IoObject * io_plane = IoObject_getSlot_(io_particulars, get_plane);

	me->entity = (EntityGetterPtr)malloc(sizeof(EntityGetter));
	me->x = (NumericPtr)malloc(sizeof(Numeric));
	me->y = (NumericPtr)malloc(sizeof(Numeric));
	me->plane = (PlaneGetterPtr)malloc(sizeof(PlaneGetter));
	me->started = false;

	IoHandler_process(io_handler, io_entity, me->entity);
	IoHandler_process(io_handler, io_x, me->x);
	IoHandler_process(io_handler, io_y, me->y);
	IoHandler_process(io_handler, io_plane, me->plane);
}

bool EntityJourneyToAction_run(void * me_void) {
	EntityJourneyToActionPtr me = (EntityJourneyToActionPtr)me_void;
	EntityPtr entity = EntityGetter_evaluate(me->entity);
	MappedComponentPtr mapped = (MappedComponentPtr)Entity_fetch_component(
		entity, MAPPED_COMPONENT
	);
	JourneyComponentPtr journey = (JourneyComponentPtr)Entity_fetch_component(
		entity, JOURNEY_COMPONENT
	);

	double x = Numeric_evaluate(me->x);
	double y = Numeric_evaluate(me->y);

	int int_x = (int)x;
	int int_y = (int)y;

	if (!me->started) {
		JourneyComponent_journey_to(journey, Plane_get_tile(
			PlaneGetter_evaluate(me->plane), int_x, int_y
		), x, y);
		me->started = true;
	}

	return mapped->x == x && mapped->y == y;
}

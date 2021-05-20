#include <stdarg.h>

#include "EntityJourneyToAction.h"

void EntityJourneyToAction_init(void * me_void, va_list * args) {
	EntityJourneyToActionPtr me = (EntityJourneyToActionPtr)me_void;
	me->entity = va_arg(*args, EntityGetterPtr);
	me->x = va_arg(*args, NumericPtr);
	me->y = va_arg(*args, NumericPtr);
	me->plane = va_arg(*args, PlanePtr);
	me->started = false;
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
		JourneyComponent_journey_to(
			journey, Plane_get_tile(me->plane, int_x, int_y), x, y
		);
		me->started = true;
	}

	return mapped->x == x && mapped->y == y;
}

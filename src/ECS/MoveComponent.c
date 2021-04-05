#include <stdarg.h>
#include <stdio.h>

#include "MoveComponent.h"

ComponentPtr MoveComponent_init(void * me_void, EntityPtr entity, va_list * args) {
	MoveComponentPtr me = (MoveComponentPtr)me_void;
	ComponentPtr me_component = (ComponentPtr)me;
	Component_init(me_component, entity, MOVE_COMPONENT);
	me->vx = 0;
	me->vy = 0;
	me->old_vx = 0;
	me->old_vy = 0;
	return me_component;
}

void MoveComponent_update(void * me_void) {
	MoveComponentPtr me = (MoveComponentPtr)me_void;
	EntityPtr entity = ((ComponentPtr)me)->entity;
	MappedComponentPtr mapped = Entity_fetch_component(
		entity, MAPPED_COMPONENT
	);
	mapped->x += me->vx;
	mapped->y += me->vy;
}

void MoveComponent_draw(void * me_void) {

}

void MoveComponent_delete(void * me_void) {

}

void MoveComponent_stop_moving(MoveComponentPtr me) {
	me->old_vx = (float)me->vx;
	me->old_vx = (float)me->vx;
	me->vx = 0.0f;
	me->vy = 0.0f;
}

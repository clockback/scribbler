#ifndef __MOVE_COMPONENT_H__
#define __MOVE_COMPONENT_H__

#include "ECS.h"
#include "MappedComponent.h"

typedef struct {
	Component base_component;
	double vx;
	double vy;

	float old_vx;
	float old_vy;

	MappedComponentPtr mapped_component;
} MoveComponent;

typedef MoveComponent * MoveComponentPtr;

ComponentPtr MoveComponent_init(
	void * me_void, EntityPtr entity, va_list * args
);
void MoveComponent_update(void * me_void);
void MoveComponent_draw(void * me_void);
void MoveComponent_delete(void * me_void);

void MoveComponent_stop_moving(MoveComponentPtr me);

#endif

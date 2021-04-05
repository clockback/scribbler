#ifndef __MAPPED_COMPONENT_H__
#define __MAPPED_COMPONENT_H__

#include "ECS.h"
#include "../objects/room.h"

typedef struct {
	Component base_component;
	double x;
	double y;
	PlanePtr plane;
} MappedComponent;

typedef MappedComponent * MappedComponentPtr;

ComponentPtr MappedComponent_init(
	void * me_void, EntityPtr entity, va_list * args
);
void MappedComponent_update(void * me_void);
void MappedComponent_draw(void * me_void);
void MappedComponent_delete(void * me_void);

#endif

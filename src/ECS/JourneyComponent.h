#ifndef __JOURNEY_COMPONENT_H__
#define __JOURNEY_COMPONENT_H__

#include "ECS.h"
#include "MappedComponent.h"
#include "MoveComponent.h"
#include "../objects/room.h"

typedef struct {
	Component base_component;
	MappedComponentPtr mapped;
	MoveComponentPtr move;
	TilePtr target_tile;
	double target_x;
	double target_y;
	RoomPtr room;
	int path_length;
	TilePtr * path_tiles;
	PlanePtr * path_planes;
} JourneyComponent;

typedef JourneyComponent * JourneyComponentPtr;

ComponentPtr JourneyComponent_init(
	void * me_void, EntityPtr entity, va_list * args
);
void JourneyComponent_update(void * me_void);
void JourneyComponent_draw(void * me_void);
void JourneyComponent_delete(void * me_void);

void JourneyComponent_journey_to(
	JourneyComponentPtr me, TilePtr tile, double x, double y
);
void JourneyComponent_dijkstra(JourneyComponentPtr me);

#endif

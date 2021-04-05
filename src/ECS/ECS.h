#ifndef __ECS_H__
#define __ECS_H__

#include <stdbool.h>
#include <stddef.h>

#include "SDL2/SDL.h"

#include "../groups.h"
#include "../objects/graphics.h"


typedef enum {
	MAPPED_COMPONENT,
	MOVE_COMPONENT,
	SPRITE_COMPONENT,
	JOURNEY_COMPONENT,
	ANIMATE_COMPONENT,
	MAX_COMPONENTS
} component_types;

typedef size_t ComponentID;
typedef size_t Group;

typedef struct Component Component;
typedef struct ComponentType ComponentType;
typedef struct Entity Entity;
typedef struct System System;

typedef Component * ComponentPtr;
typedef ComponentType * ComponentTypePtr;
typedef Entity * EntityPtr;
typedef System * SystemPtr;

/* Component-related types for use by Entity class */
typedef bool ComponentBitSet[MAX_COMPONENTS];
typedef ComponentPtr ComponentArray[MAX_COMPONENTS];
typedef size_t GroupSet[MAX_GROUPS];
typedef bool GroupBitSet[MAX_GROUPS];

/* Component class */
struct Component {
    EntityPtr entity;
    ComponentID type;
};

void Component_init(ComponentPtr me, EntityPtr entity, ComponentID type);
void Component_update(ComponentPtr me);
void Component_draw(ComponentPtr me);
void Component_delete(ComponentPtr me);


/* Entity class */
struct Entity {
	bool active;
	ComponentBitSet has_component;
	void * components[MAX_COMPONENTS];
	SystemPtr system;
	GroupBitSet in_group;
	ScreenPtr screen;
};

void Entity_init(EntityPtr me, SystemPtr SystemPtr, ScreenPtr screen);
void * Entity_add_component(EntityPtr me, ComponentID type, int no_args, ...);
void * Entity_fetch_component(EntityPtr me, ComponentID type);
void Entity_add_group(EntityPtr me, Group group);
void Entity_delete_group(EntityPtr me, Group group);
bool Entity_has_component(EntityPtr me, ComponentID type);
void Entity_update(EntityPtr me);
void Entity_draw(EntityPtr me);
void Entity_delete(EntityPtr me);


/* System class */
struct System {
    EntityPtr * entities;
    int no_entities;
    EntityPtr * grouped_entities[MAX_GROUPS];
    size_t grouped_entity_sizes[MAX_GROUPS];
	ScreenPtr screen;
};

void System_init(SystemPtr me, ScreenPtr screen);
EntityPtr System_add_entity(SystemPtr me);
void System_add_to_group(SystemPtr me, EntityPtr entity, Group group);
EntityPtr * System_get_group(SystemPtr me, Group group);
size_t System_get_group_size(SystemPtr me, Group group);
void System_update(SystemPtr me);
void System_draw(SystemPtr me);
void System_refresh(SystemPtr me);
SDL_Texture * System_load_sprite(
	SystemPtr me, const char * path, int * width, int * height
);

#endif

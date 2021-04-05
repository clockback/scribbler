#ifndef __COMPONENTS_H__
#define __COMPONENTS_H__

#include "MappedComponent.h"
#include "MoveComponent.h"
#include "SpriteComponent.h"
#include "JourneyComponent.h"
#include "AnimateComponent.h"
#include "ECS.h"

extern ComponentPtr (*init_for_component_functions[MAX_COMPONENTS]) (
	void* me_void, EntityPtr entity, va_list * args
);
extern size_t component_sizes[MAX_COMPONENTS];
extern void (*update_for_component_functions[MAX_COMPONENTS]) (void* me_void);
extern void (*draw_for_component_functions[MAX_COMPONENTS]) (void* me_void);
extern void (*delete_for_component_functions[MAX_COMPONENTS]) (void* me_void);


void pre_init_functions();
void pre_init_sizes();
void pre_update_functions();
void pre_draw_functions();
void init_components();

#endif

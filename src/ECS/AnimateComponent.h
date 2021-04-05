#ifndef __ANIMATE_COMPONENT_H__
#define __ANIMATE_COMPONENT_H__

#include "ECS.h"
#include "MappedComponent.h"
#include "MoveComponent.h"

typedef enum {
	STAND_FORWARDS,
	STAND_BACKWARDS,
	STAND_LEFT,
	STAND_RIGHT,
	WALK_FORWARDS_1,
	WALK_FORWARDS_2,
	WALK_FORWARDS_3,
	WALK_FORWARDS_4,
	WALK_FORWARDS_5,
	WALK_FORWARDS_6,
	WALK_BACKWARDS_1,
	WALK_BACKWARDS_2,
	WALK_BACKWARDS_3,
	WALK_BACKWARDS_4,
	WALK_BACKWARDS_5,
	WALK_BACKWARDS_6,
	WALK_LEFT_1,
	WALK_LEFT_2,
	WALK_LEFT_3,
	WALK_LEFT_4,
	WALK_LEFT_5,
	WALK_LEFT_6,
	WALK_RIGHT_1,
	WALK_RIGHT_2,
	WALK_RIGHT_3,
	WALK_RIGHT_4,
	WALK_RIGHT_5,
	WALK_RIGHT_6,
	MAX_ANIMATION_FRAMES
} animation_frames;

typedef struct {
	Component base_component;
	SDL_Texture * images[MAX_ANIMATION_FRAMES];
	SDL_Rect * source_rects[MAX_ANIMATION_FRAMES];
	SDL_Rect * dest_rect;
	MappedComponentPtr mapped;
	int scale;
	const char * directory;
	size_t current;
	bool display;
	int i;
	int count;
} AnimateComponent;

typedef AnimateComponent * AnimateComponentPtr;

ComponentPtr AnimateComponent_init(
	void * me_void, EntityPtr entity, va_list * args
);
void AnimateComponent_update(void * me_void);
void AnimateComponent_draw(void * me_void);
void AnimateComponent_delete(void * me_void);

void AnimateComponent_load_sprite(
	AnimateComponentPtr me, size_t animation, const char * filename
);
void AnimateComponent_use_sprite(AnimateComponentPtr me, size_t animation);
void AnimateComponent_set_display(AnimateComponentPtr me, bool display);

#endif

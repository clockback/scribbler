#ifndef __SPRITE_COMPONENT_H__
#define __SPRITE_COMPONENT_H__

#include "ECS.h"
#include "MappedComponent.h"

typedef struct {
	Component base_component;
	SDL_Texture * image;
	SDL_Rect * src_rect;
	SDL_Rect * dest_rect;
	MappedComponentPtr mapped;
	double scale;
} SpriteComponent;

typedef SpriteComponent * SpriteComponentPtr;

ComponentPtr SpriteComponent_init(
	void * me_void, EntityPtr entity, va_list * args
);
void SpriteComponent_update(void * me_void);
void SpriteComponent_draw(void * me_void);
void SpriteComponent_delete(void * me_void);

void SpriteComponent_scale(SpriteComponentPtr me, double scale);

#endif

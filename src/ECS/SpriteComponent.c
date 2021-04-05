#include <stdarg.h>
#include <stdio.h>

#include "SpriteComponent.h"
#include "ECS.h"

ComponentPtr SpriteComponent_init(
	void * me_void, EntityPtr entity, va_list * args
) {
	SpriteComponentPtr me = (SpriteComponentPtr)me_void;
	ComponentPtr me_component = (ComponentPtr)me;
	Component_init(me_component, entity, SPRITE_COMPONENT);

	int width, height;

	me->image = System_load_sprite(
		me_component->entity->system, va_arg(*args, const char *), &width,
		&height
	);

	int scale = va_arg(*args, int);

	me->src_rect = (SDL_Rect *) malloc(sizeof(SDL_Rect));
	me->src_rect->x = 0;
	me->src_rect->y = 0;
	me->src_rect->w = width;
	me->src_rect->h = height;

	me->mapped = (MappedComponentPtr)Entity_fetch_component(
		entity, MAPPED_COMPONENT
	);

	me->dest_rect = (SDL_Rect *) malloc(sizeof(SDL_Rect));
	me->dest_rect->x = me->mapped->x;
	me->dest_rect->y = me->mapped->y;
	me->dest_rect->w = width * scale;
	me->dest_rect->h = height * scale;

	return me_component;
}

void SpriteComponent_update(void * me_void) {
}

void SpriteComponent_draw(void * me_void) {
	SpriteComponentPtr me = (SpriteComponentPtr)me_void;
	ComponentPtr me_component = (ComponentPtr)me;
	if (me->mapped == NULL) {
		me->dest_rect->x = me->mapped->x;
		me->dest_rect->y = me->mapped->y;
	}
	else {
		ScreenPtr screen = me_component->entity->screen;
		int layer_x = Room_get_point_x(
			me->mapped->plane->room, screen, me->mapped->x, me->mapped->y
		);
		int layer_y = Room_get_point_y(
			me->mapped->plane->room, screen, me->mapped->x, me->mapped->y,
			Plane_get_height(me->mapped->plane, me->mapped->x, me->mapped->y)
		);

		me->dest_rect->x = (int)(layer_x - me->dest_rect->w / 2);
		me->dest_rect->y = (int)(layer_y - me->dest_rect->h);
	}

	SDL_RenderCopy(
		me_component->entity->screen->rend, me->image, me->src_rect,
		me->dest_rect
	);
}

void SpriteComponent_delete(void * me_void) {

}

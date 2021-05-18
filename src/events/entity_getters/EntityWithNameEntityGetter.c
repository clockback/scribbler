#include <stdarg.h>

#include "EntityWithNameEntityGetter.h"

void EntityWithNameEntityGetter_init(void * me_void, va_list * args) {
	EntityWithNameEntityGetterPtr me = (EntityWithNameEntityGetterPtr)me_void;
	me->game = va_arg(*args, GamePtr);
	me->name = va_arg(*args, StringPtr);
}

EntityPtr EntityWithNameEntityGetter_evaluate(void * me_void) {
	EntityWithNameEntityGetterPtr me = (EntityWithNameEntityGetterPtr)me_void;
	return Game_find_entity(me->game, String_evaluate(me->name));
}

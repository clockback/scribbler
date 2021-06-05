#include <stdarg.h>

#include "EntityWithNameEntityGetter.h"

void EntityWithNameEntityGetter_init(
	void * me_void, IoObject * io_particulars, IoHandler * io_handler,
	GamePtr game
) {
	EntityWithNameEntityGetterPtr me = (EntityWithNameEntityGetterPtr)me_void;
	me->game = game;

	IoSymbol * get_name = IoState_symbolWithCString_(
		io_handler->iostate, "name"
	);

	IoObject * io_name = IoObject_getSlot_(io_particulars, get_name);

	me->name = (StringPtr)malloc(sizeof(String));

	IoHandler_process(io_handler, io_name, me->name);
}

EntityPtr EntityWithNameEntityGetter_evaluate(void * me_void) {
	EntityWithNameEntityGetterPtr me = (EntityWithNameEntityGetterPtr)me_void;
	return Game_find_entity(me->game, String_evaluate(me->name));
}

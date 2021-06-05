#include <stdarg.h>

#include "./InteractEntityTrigger.h"

void InteractEntityTrigger_init(void * me_void, va_list * args) {
	InteractEntityTriggerPtr me = (InteractEntityTriggerPtr)me_void;
	me->object = va_arg(*args, EntityPtr);
}

void InteractEntityTrigger_listener_init(
	void * me_void, IoObject * io_particulars, IoHandler * io_handler,
	GamePtr game
) {
	InteractEntityTriggerPtr me = (InteractEntityTriggerPtr)me_void;
	IoSymbol * get_entity = IoState_symbolWithCString_(
		io_handler->iostate, "entity"
	);
	IoSymbol * get_name = IoState_symbolWithCString_(
		io_handler->iostate, "name"
	);
	IoObject * io_entity = IoObject_getSlot_(io_particulars, get_entity);
	char * name = IoSeq_asCString(
		IoObject_getSlot_(io_entity, get_name)
	);
	me->object = Game_find_entity(game, name);
}

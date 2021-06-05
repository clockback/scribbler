#include <stdarg.h>

#include "RawEntityEntityGetter.h"

void RawEntityEntityGetter_init(
	void * me_void, IoObject * io_particulars, IoHandler * io_handler,
	GamePtr game
) {
	RawEntityEntityGetterPtr me = (RawEntityEntityGetterPtr)me_void;

	IoSymbol * get_name = IoState_symbolWithCString_(
		io_handler->iostate, "name"
	);
	IoObject * io_name = IoObject_getSlot_(io_particulars, get_name);

	char * name = IoSeq_asCString(io_name);
	me->entity = Game_find_entity(game, name);
}

EntityPtr RawEntityEntityGetter_evaluate(void * me_void) {
	RawEntityEntityGetterPtr me = (RawEntityEntityGetterPtr)me_void;
	return me->entity;
}

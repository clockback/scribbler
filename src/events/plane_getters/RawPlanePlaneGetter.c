#include <stdarg.h>

#include "RawPlanePlaneGetter.h"

void RawPlanePlaneGetter_init(
	void * me_void, IoObject * io_particulars, IoHandler * io_handler,
	GamePtr game
) {
	RawPlanePlaneGetterPtr me = (RawPlanePlaneGetterPtr)me_void;

	IoSymbol * get_name = IoState_symbolWithCString_(
		io_handler->iostate, "name"
	);
	IoSymbol * get_room = IoState_symbolWithCString_(
		io_handler->iostate, "room"
	);

	IoObject * io_name = IoObject_getSlot_(io_particulars, get_name);
	IoObject * io_room = IoObject_getSlot_(io_particulars, get_room);
	IoObject * io_room_name = IoObject_getSlot_(io_room, get_name);

	char * name = IoSeq_asCString(io_name);
	char * room_name = IoSeq_asCString(io_room_name);

	me->plane = Room_find_plane(Game_find_room(game, room_name), name);
}

PlanePtr RawPlanePlaneGetter_evaluate(void * me_void) {
	RawPlanePlaneGetterPtr me = (RawPlanePlaneGetterPtr)me_void;
	return me->plane;
}

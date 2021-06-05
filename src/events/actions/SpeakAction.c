#include <stdarg.h>

#include "SpeakAction.h"

void SpeakAction_init(
	void * me_void, IoObject * io_particulars, IoHandler * io_handler,
	GamePtr game
) {
	SpeakActionPtr me = (SpeakActionPtr)me_void;

	IoSymbol * get_entity = IoState_symbolWithCString_(
		io_handler->iostate, "entity"
	);
	IoSymbol * get_text = IoState_symbolWithCString_(
		io_handler->iostate, "text"
	);
	IoSymbol * get_duration = IoState_symbolWithCString_(
		io_handler->iostate, "duration"
	);

	IoObject * io_entity = IoObject_getSlot_(io_particulars, get_entity);
	IoObject * io_text = IoObject_getSlot_(io_particulars, get_text);
	IoObject * io_duration = IoObject_getSlot_(io_particulars, get_duration);

	me->entity = (EntityGetterPtr)malloc(sizeof(EntityGetter));
	me->text = (StringPtr)malloc(sizeof(String));
	me->duration = (NumericPtr)malloc(sizeof(Numeric));

	IoHandler_process(io_handler, io_entity, me->entity);
	IoHandler_process(io_handler, io_text, me->text);
	IoHandler_process(io_handler, io_duration, me->duration);

	me->started = false;
	me->i = -1;
}

bool SpeakAction_run(void * me_void) {
	SpeakActionPtr me = (SpeakActionPtr)me_void;

	if (me->i == -1) {
		me->i = (int)Numeric_evaluate(me->duration);
	}

	if (!me->started) {
		me->started = true;
		SpeakComponentPtr speak = (SpeakComponentPtr)Entity_fetch_component(
			EntityGetter_evaluate(me->entity), SPEAK_COMPONENT
		);
		SpeakComponent_speak(speak, String_evaluate(me->text), me->i);
	}

	return (me->i --) <= 0;
}

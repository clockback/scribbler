#include <stdarg.h>

#include "SpeakAction.h"

void SpeakAction_init(void * me_void, va_list * args) {
	SpeakActionPtr me = (SpeakActionPtr)me_void;
	me->entity = va_arg(*args, EntityGetterPtr);
	me->text = va_arg(*args, StringPtr);
	me->duration = va_arg(*args, NumericPtr);
	me->started = false;
	me->i = (int)Numeric_evaluate(me->duration);
}

bool SpeakAction_run(void * me_void) {
	SpeakActionPtr me = (SpeakActionPtr)me_void;

	if (!me->started) {
		me->started = true;
		SpeakComponentPtr speak = (SpeakComponentPtr)Entity_fetch_component(
			EntityGetter_evaluate(me->entity), SPEAK_COMPONENT
		);
		SpeakComponent_speak(speak, String_evaluate(me->text), me->i);
	}

	return (me->i --) <= 0;
}

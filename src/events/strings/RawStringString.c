#include <stdarg.h>

#include "RawStringString.h"

void RawStringString_init(
	void * me_void, IoObject * io_particulars, IoHandler * io_handler,
	GamePtr game
) {
	RawStringStringPtr me = (RawStringStringPtr)me_void;

	char * string = IoSeq_asCString(io_particulars);
	me->string = (char *) malloc(strlen(string) * (sizeof(char) + 1));
	strcpy(me->string, string);
}

char * RawStringString_evaluate(void * me_void) {
	RawStringStringPtr me = (RawStringStringPtr)me_void;
	return me->string;
}

#include <stdarg.h>

#include "RawStringString.h"

void RawStringString_init(void * me_void, va_list * args) {
	RawStringStringPtr me = (RawStringStringPtr)me_void;

	char * string = va_arg(*args, char *);
	me->string = (char *) malloc(strlen(string) * (sizeof(char) + 1));
	strcpy(me->string, string);
}

char * RawStringString_evaluate(void * me_void) {
	RawStringStringPtr me = (RawStringStringPtr)me_void;
	return me->string;
}

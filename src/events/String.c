#include "String.h"


size_t string_sizes[MAX_STRINGS];

void (*init_for_string_functions[MAX_STRINGS]) (
	void * me_void, va_list * args
);

char * (*evaulate_for_string_functions[MAX_STRINGS]) (void * me_void);

void String_init(StringPtr me, StringType type, va_list * args) {
	me->type = type;
	me->particulars = (void *) malloc(string_sizes[type]);
	init_for_string_functions[type](me->particulars, args);
}

char * String_evaluate(StringPtr me) {
	return evaulate_for_string_functions[me->type](me->particulars);
}

void pre_init_string_functions() {
	init_for_string_functions[RAW_STRING] = RawStringString_init;
}

void pre_init_string_sizes() {
	string_sizes[RAW_STRING] = sizeof(RawStringString);
}

void pre_init_string_evaluate() {
	evaulate_for_string_functions[RAW_STRING] = RawStringString_evaluate;
}

void init_strings() {
	pre_init_string_functions();
	pre_init_string_sizes();
	pre_init_string_evaluate();
}

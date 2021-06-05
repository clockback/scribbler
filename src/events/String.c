#include "String.h"


size_t string_sizes[MAX_STRINGS];

void (*init_for_string_functions[MAX_STRINGS]) (
	void * me_void, IoObject * io_particulars, IoHandler * io_handler,
	GamePtr game
);

char * (*evaulate_for_string_functions[MAX_STRINGS]) (void * me_void);

void String_init(
	StringPtr me, IoObject * base, IoHandler * io_handler, GamePtr game
) {
	const char * string_type_name = IoObject_name(base);

	if (ISSEQ(base)) {
		me->type = RAW_STRING;
	}
	else {
		printf("Unknown string type: %s\n", string_type_name);
		IoObject_print(base);
		printf("\n");
		exit(-1);
	}

	me->particulars = (void *) malloc(string_sizes[me->type]);
	init_for_string_functions[me->type](
		me->particulars, base, io_handler, game
	);
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

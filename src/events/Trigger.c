#include "Trigger.h"


size_t trigger_sizes[MAX_TRIGGERS];

void (*init_for_trigger_functions[MAX_TRIGGERS]) (
	void * me_void, va_list * args
);

void (*init_for_listener_functions[MAX_TRIGGERS]) (
	void * me_void, IoObject * io_particulars, IoHandler * io_handler,
	GamePtr game
);

void pre_init_trigger_functions() {
	init_for_trigger_functions[INTERACT_ENTITY] = InteractEntityTrigger_init;
}

void pre_init_listener_functions() {
	init_for_listener_functions[INTERACT_ENTITY] = (
		InteractEntityTrigger_listener_init
	);
}

void pre_init_trigger_sizes() {
	trigger_sizes[INTERACT_ENTITY] = sizeof(InteractEntityTrigger);
}

void init_triggers() {
	pre_init_trigger_functions();
	pre_init_listener_functions();
	pre_init_trigger_sizes();
}

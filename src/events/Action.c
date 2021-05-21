#include "Action.h"


size_t action_sizes[MAX_ACTIONS];
void (*init_for_action_functions[MAX_ACTIONS]) (
	void * me_void, va_list * args
);
bool (*run_for_action_functions[MAX_ACTIONS]) (void * me_void);


void pre_init_action_functions() {
	init_for_action_functions[ENTITY_JOURNEY_TO] = EntityJourneyToAction_init;
	init_for_action_functions[SET_USER_INPUT] = SetUserInputAction_init;
	init_for_action_functions[WAIT] = WaitAction_init;
	init_for_action_functions[SPEAK] = SpeakAction_init;
}

void pre_init_action_sizes() {
	action_sizes[ENTITY_JOURNEY_TO] = sizeof(EntityJourneyToAction);
	action_sizes[SET_USER_INPUT] = sizeof(SetUserInputAction);
	action_sizes[WAIT] = sizeof(WaitAction);
	action_sizes[SPEAK] = sizeof(SpeakAction);
}

void pre_init_action_run() {
	run_for_action_functions[ENTITY_JOURNEY_TO] = EntityJourneyToAction_run;
	run_for_action_functions[SET_USER_INPUT] = SetUserInputAction_run;
	run_for_action_functions[WAIT] = WaitAction_run;
	run_for_action_functions[SPEAK] = SpeakAction_run;
}

void init_actions() {
	pre_init_action_functions();
	pre_init_action_sizes();
	pre_init_action_run();
}

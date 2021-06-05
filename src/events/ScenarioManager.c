#include "stdio.h"

#include "ScenarioManager.h"
#include "Trigger.h"

void Trigger_init(TriggerPtr me, TriggerType type, va_list * args) {
	me->type = type;
	me->particulars = (void *) malloc(trigger_sizes[type]);
	init_for_trigger_functions[type](me->particulars, args);
}

void Trigger_delete(TriggerPtr me) {
	free(me->particulars);
}

void Listener_init(
	ListenerPtr me, IoObject * base, IoHandler * io_handler, GamePtr game,
	int priority
) {
	const char * trigger_type_name = IoObject_name(base);

	if (strcmp(trigger_type_name, "InteractEntityTriggerObj") == 0) {
		me->type = INTERACT_ENTITY;
	}
	else {
		printf("Unknown trigger type: %s\n", trigger_type_name);
		IoObject_print(base);
		printf("\n");
		exit(-1);
	}

	me->particulars = (void *) malloc(trigger_sizes[me->type]);
	me->priority = priority;
	init_for_listener_functions[me->type](
		me->particulars, base, io_handler, game
	);
}

bool Listener_catch_trigger(ListenerPtr me, TriggerPtr trigger) {
	if (me->type != trigger->type) {
		return false;
	}

	return memcmp(
		me->particulars, trigger->particulars, sizeof(trigger_sizes[me->type])
	) == 0;
}

void Condition_init(
	ConditionPtr me, IoObject * base, IoHandler * io_handler, GamePtr game
) {
	const char * condition_type_name = IoObject_name(base);

	if (strcmp(condition_type_name, "NumericEqualToConditionObj") == 0) {
		me->type = NUMERIC_EQUAL_TO;
	}
	else if (strcmp(
		condition_type_name, "NumericGreaterThanConditionObj"
	) == 0) {
		me->type = NUMERIC_GREATER_THAN;
	}
	else if (strcmp(
		condition_type_name, "NumericGreaterThanOrEqualToConditionObj"
	) == 0) {
		me->type = NUMERIC_GREATER_THAN_OR_EQUAL_TO;
	}
	else if (strcmp(
		condition_type_name, "NumericLessThanConditionObj"
	) == 0) {
		me->type = NUMERIC_LESS_THAN;
	}
	else if (strcmp(
		condition_type_name, "NumericLessThanOrEqualToConditionObj"
	) == 0) {
		me->type = NUMERIC_LESS_THAN_OR_EQUAL_TO;
	}
	else {
		printf("Unknown condition type: %s\n", condition_type_name);
		IoObject_print(base);
		printf("\n");
		exit(-1);
	}

	me->particulars = (void *) malloc(condition_sizes[me->type]);
	init_for_condition_functions[me->type](
		me->particulars, base, io_handler, game
	);
}

bool Condition_check(ConditionPtr me) {
	return evaluate_for_condition_functions[me->type](me->particulars);
}

void Action_init(
	ActionPtr me, IoObject * base, IoHandler * io_handler, GamePtr game
) {
	const char * action_type_name = IoObject_name(base);

	if (strcmp(action_type_name, "EntityJourneyToActionObj") == 0) {
		me->type = ENTITY_JOURNEY_TO;
	}
	else if (strcmp(action_type_name, "SetUserInputActionObj") == 0) {
		me->type = SET_USER_INPUT;
	}
	else if (strcmp(action_type_name, "SpeakActionObj") == 0) {
		me->type = SPEAK;
	}
	else if (strcmp(action_type_name, "WaitActionObj") == 0) {
		me->type = WAIT;
	}
	else {
		printf("Unknown action type: %s\n", action_type_name);
		IoObject_print(base);
		printf("\n");
		exit(-1);
	}

	IoSymbol * get_async = IoState_symbolWithCString_(
		io_handler->iostate, "async"
	);
	IoObject * is_async = IoObject_getSlot_(base, get_async);
	if (ISNUMBER(is_async)) {
		me->is_async = (bool)IoNumber_asInt(is_async);
	}

	me->particulars = (void *) malloc(action_sizes[me->type]);
	init_for_action_functions[me->type](
		me->particulars, base, io_handler, game
	);
}

void Action_make_async(ActionPtr me) {
	me->is_async = true;
}

bool Action_run(ActionPtr me) {
	return run_for_action_functions[me->type](me->particulars);
}

void Scenario_init(
	ScenarioPtr me, size_t no_listeners, size_t no_conditions,
	size_t no_actions
) {
	me->listeners = (ListenerPtr *) malloc(no_listeners * sizeof(ListenerPtr));
	me->conditions = (ConditionPtr *) malloc(
		no_conditions * sizeof(ConditionPtr)
	);
	me->actions = (ActionPtr *) malloc(no_actions * sizeof(ActionPtr));
	me->numerics = (NumericPtr *) malloc(0);
	me->entity_getters = (EntityGetterPtr *) malloc(0);
	me->strings = (StringPtr *) malloc(0);
	me->plane_getters = (PlaneGetterPtr *) malloc(0);
	me->action_queues = (ActionQueuePtr *) malloc(0);

	me->no_listeners = no_listeners;
	me->no_conditions = no_conditions;
	me->no_actions = no_actions;
	me->no_numerics = 0;
	me->no_entity_getters = 0;
	me->no_strings = 0;
	me->no_plane_getters = 0;
	me->no_action_queues = 0;

	for (size_t i = 0; i < no_listeners; i ++) {
		me->listeners[i] = (ListenerPtr)malloc(sizeof(Listener));
	}
	for (size_t i = 0; i < no_conditions; i ++) {
		me->conditions[i] = (ConditionPtr)malloc(sizeof(Condition));
	}
	for (size_t i = 0; i < no_actions; i ++) {
		me->actions[i] = (ActionPtr)malloc(sizeof(Action));
	}

	me->active = true;
}

ActionQueuePtr Scenario_add_action_queue(ScenarioPtr me) {
	me->no_action_queues ++;
	me->action_queues = (ActionQueuePtr *) realloc(
		me->action_queues, me->no_action_queues * sizeof(ActionQueuePtr)
	);
	me->action_queues[me->no_action_queues - 1] = (ActionQueuePtr) malloc(
		sizeof(ActionPtr)
	);
	ActionQueue_init(
		me->action_queues[me->no_action_queues - 1], me->actions, me->no_actions
	);

	return me->action_queues[me->no_action_queues - 1];
}

void Scenario_check_conditions(ScenarioPtr me) {
	for (size_t i = 0; i < me->no_conditions; i ++) {
		ConditionPtr condition = me->conditions[i];
		if (!Condition_check(condition)) {
			return;
		}
	}

	Scenario_add_action_queue(me);
}

void ScenarioManager_init(ScenarioManagerPtr me) {
	me->scenarios = (ScenarioPtr *) malloc(0);
	me->no_scenarios = 0;
	me->triggers = (TriggerPtr *) malloc(0);
	me->no_triggers = 0;
	me->scenario_enabled = (bool *) malloc(0);

	init_triggers();
	init_conditions();
	init_actions();
	init_numerics();
	init_entity_getters();
	init_strings();
	init_plane_getters();
}

ScenarioPtr ScenarioManager_add_scenario(
	ScenarioManagerPtr me, size_t no_listeners, size_t no_conditions,
	size_t no_actions
) {
	me->no_scenarios ++;
	me->scenarios = (ScenarioPtr *) realloc(
		me->scenarios, me->no_scenarios * sizeof(ScenarioPtr)
	);
	me->scenarios[me->no_scenarios - 1] = (ScenarioPtr) malloc(
		sizeof(Scenario)
	);

	Scenario_init(
		me->scenarios[me->no_scenarios - 1], no_listeners, no_conditions,
		no_actions
	);

	return me->scenarios[me->no_scenarios - 1];
}

TriggerPtr ScenarioManager_queue_trigger(
	ScenarioManagerPtr me, TriggerType type, size_t no_args, ...
) {
	me->no_triggers ++;
	me->triggers = (TriggerPtr *) realloc(
		me->triggers, me->no_triggers * sizeof(TriggerPtr)
	);
	me->triggers[me->no_triggers - 1] = (TriggerPtr) malloc(sizeof(Trigger));
	TriggerPtr trigger = me->triggers[me->no_triggers - 1];

	va_list args;
	va_start(args, no_args);
	Trigger_init(trigger, type, &args);
	va_end(args);

	return trigger;
}

void ScenarioManager_handle_scenarios(ScenarioManagerPtr me) {
	for (size_t i = 0; i < me->no_scenarios; i ++) {
		ScenarioPtr scenario = me->scenarios[i];
		for (size_t j = 0; j < scenario->no_listeners; j ++) {
			ListenerPtr listener = scenario->listeners[j];
			for (size_t k = 0; k < me->no_triggers; k ++) {
				TriggerPtr trigger = me->triggers[k];
				if (Listener_catch_trigger(listener, trigger)) {
					Scenario_check_conditions(scenario);
				}
			}
		}
	}
}

void ScenarioManager_clean_triggers(ScenarioManagerPtr me) {
	for (size_t i = 0; i < me->no_triggers; i ++) {
		TriggerPtr trigger = me->triggers[i];
		Trigger_delete(trigger);
		free(trigger);
	}
	me->no_triggers = 0;
}

void ScenarioManager_run_action_queues(ScenarioManagerPtr me) {
	for (size_t i = 0; i < me->no_scenarios; i ++) {
		ScenarioPtr scenario = me->scenarios[i];
		for (size_t j = 0; j < scenario->no_action_queues; j ++) {
			bool depleted = ActionQueue_run(scenario->action_queues[j]);
			if (depleted) {
				free(scenario->action_queues[j]);
				scenario->no_action_queues --;
				for (size_t k = j; k < scenario->no_action_queues; k ++) {
					scenario->action_queues[k] = scenario->action_queues[k - 1];
				}
				scenario->action_queues = (ActionQueuePtr *) realloc(
					scenario->action_queues,
					scenario->no_action_queues * sizeof(ActionQueuePtr)
				);
			}
		}
	}
}

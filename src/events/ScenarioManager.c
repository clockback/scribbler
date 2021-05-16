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
	ListenerPtr me, TriggerType type, int priority, va_list * args
) {
	me->type = type;
	me->priority = priority;
	me->particulars = (void *) malloc(trigger_sizes[type]);
	init_for_trigger_functions[type](me->particulars, args);
}

bool Listener_catch_trigger(ListenerPtr me, TriggerPtr trigger) {
	if (me->type != trigger->type) {
		return false;
	}

	return memcmp(
		me->particulars, trigger->particulars, sizeof(InteractEntityTrigger)
	) == 0;
}

void Condition_init(ConditionPtr me, ConditionType type) {
	me->type = type;
}

void Action_init(ActionPtr me, ActionType type) {
	me->type = type;
}

void Scenario_init(ScenarioPtr me) {
	me->listeners = (ListenerPtr *) malloc(0);
	me->conditions = (ConditionPtr *) malloc(0);
	me->actions = (ActionPtr *) malloc(0);

	me->no_listeners = 0;
	me->no_conditions = 0;
	me->no_actions = 0;

	me->active = true;
}

ListenerPtr Scenario_add_listener(
	ScenarioPtr me, TriggerType type, int priority, size_t no_args, ...
) {
	me->no_listeners ++;
	me->listeners = (ListenerPtr *) realloc(
		me->listeners, me->no_listeners * sizeof(ListenerPtr)
	);
	me->listeners[me->no_listeners - 1] = (ListenerPtr) malloc(
		sizeof(Listener)
	);

	va_list args;
	va_start(args, no_args);
	Listener_init(me->listeners[me->no_listeners - 1], type, priority, &args);
	va_end(args);

	return me->listeners[me->no_listeners - 1];
}

void Scenario_check_conditions(ScenarioPtr me) {
	printf("Scenario triggered. Check conditions.\n");
}

void ScenarioManager_init(ScenarioManagerPtr me) {
	me->scenarios = (ScenarioPtr *) malloc(0);
	me->no_scenarios = 0;
	me->triggers = (TriggerPtr *) malloc(0);
	me->no_triggers = 0;
	me->scenario_enabled = (bool *) malloc(0);

	init_triggers();
}

ScenarioPtr ScenarioManager_add_scenario(ScenarioManagerPtr me) {
	me->no_scenarios ++;
	me->scenarios = (ScenarioPtr *) realloc(
		me->scenarios, me->no_scenarios * sizeof(ScenarioPtr)
	);
	me->scenarios[me->no_scenarios - 1] = (ScenarioPtr) malloc(
		sizeof(Scenario)
	);

	Scenario_init(me->scenarios[me->no_scenarios - 1]);

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

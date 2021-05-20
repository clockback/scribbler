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
		me->particulars, trigger->particulars, sizeof(trigger_sizes[me->type])
	) == 0;
}

void Condition_init(ConditionPtr me, ConditionType type, va_list * args) {
	me->type = type;
	me->particulars = (void *) malloc(condition_sizes[type]);
	init_for_condition_functions[type](me->particulars, args);
}

bool Condition_check(ConditionPtr me) {
	return evaluate_for_condition_functions[me->type](me->particulars);
}

void Action_init(
	ActionPtr me, ActionType type, va_list * args
) {
	me->type = type;
	me->is_async = false;
	me->particulars = (void *) malloc(action_sizes[type]);
	init_for_action_functions[type](me->particulars, args);
}

void Action_make_async(ActionPtr me) {
	me->is_async = true;
}

bool Action_run(ActionPtr me) {
	return run_for_action_functions[me->type](me->particulars);
}

void Scenario_init(ScenarioPtr me) {
	me->listeners = (ListenerPtr *) malloc(0);
	me->conditions = (ConditionPtr *) malloc(0);
	me->actions = (ActionPtr *) malloc(0);
	me->numerics = (NumericPtr *) malloc(0);
	me->entity_getters = (EntityGetterPtr *) malloc(0);
	me->strings = (StringPtr *) malloc(0);
	me->action_queues = (ActionQueuePtr *) malloc(0);

	me->no_listeners = 0;
	me->no_conditions = 0;
	me->no_actions = 0;
	me->no_numerics = 0;
	me->no_entity_getters = 0;
	me->no_strings = 0;
	me->no_action_queues = 0;

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

ConditionPtr Scenario_add_condition(
	ScenarioPtr me, ConditionType type, size_t no_args, ...
) {
	me->no_conditions ++;
	me->conditions = (ConditionPtr *) realloc(
		me->conditions, me->no_conditions * sizeof(ConditionPtr)
	);
	me->conditions[me->no_conditions - 1] = (ConditionPtr) malloc(
		sizeof(Condition)
	);

	va_list args;
	va_start(args, no_args);
	Condition_init(me->conditions[me->no_conditions - 1], type, &args);
	va_end(args);

	return me->conditions[me->no_conditions - 1];
}

ActionPtr Scenario_add_action(
	ScenarioPtr me, ActionType type, size_t no_args, ...
) {
	me->no_actions ++;
	me->actions = (ActionPtr *) realloc(
		me->actions, me->no_actions * sizeof(ActionPtr)
	);
	me->actions[me->no_actions - 1] = (ActionPtr) malloc(sizeof(Action));

	va_list args;
	va_start(args, no_args);
	Action_init(me->actions[me->no_actions - 1], type, &args);
	va_end(args);

	return me->actions[me->no_actions - 1];
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

NumericPtr Scenario_add_numeric(
	ScenarioPtr me, NumericType type, size_t no_args, ...
) {
	me->no_numerics ++;
	me->numerics = (NumericPtr *) realloc(
		me->numerics, me->no_numerics * sizeof(NumericPtr)
	);
	me->numerics[me->no_numerics - 1] = (NumericPtr) malloc(sizeof(Numeric));

	va_list args;
	va_start(args, no_args);
	Numeric_init(me->numerics[me->no_numerics - 1], type, &args);
	va_end(args);

	return me->numerics[me->no_numerics - 1];
}

EntityGetterPtr Scenario_add_entity_getter(
	ScenarioPtr me, EntityGetterType type, size_t no_args, ...
) {
	me->no_entity_getters ++;
	me->entity_getters = (EntityGetterPtr *) realloc(
		me->entity_getters, me->no_entity_getters * sizeof(EntityGetterPtr)
	);
	me->entity_getters[me->no_entity_getters - 1] = (EntityGetterPtr) malloc(
		sizeof(EntityGetter)
	);

	va_list args;
	va_start(args, no_args);
	EntityGetter_init(
		me->entity_getters[me->no_entity_getters - 1], type, &args
	);
	va_end(args);

	return me->entity_getters[me->no_entity_getters - 1];
}

StringPtr Scenario_add_string(
	ScenarioPtr me, StringType type, size_t no_args, ...
) {
	me->no_strings ++;
	me->strings = (StringPtr *) realloc(
		me->strings, me->no_strings * sizeof(StringPtr)
	);
	me->strings[me->no_strings - 1] = (StringPtr) malloc(sizeof(String));

	va_list args;
	va_start(args, no_args);
	String_init(me->strings[me->no_strings - 1], type, &args);
	va_end(args);

	return me->strings[me->no_strings - 1];
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

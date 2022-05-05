/**
 * Scribbler
 * Copyright (C) 2022 Elliot Paton-Simpson
 *
 * This file is part of Scribbler.
 *
 *  Scribbler is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Scribbler is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Scribbler.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef __SCENARIOMANAGER_H__
#define __SCENARIOMANAGER_H__

#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>


typedef struct ScenarioManager ScenarioManager;
typedef ScenarioManager * ScenarioManagerPtr;

typedef struct Trigger Trigger;
typedef Trigger * TriggerPtr;

typedef struct Listener Listener;
typedef Listener * ListenerPtr;

typedef struct Condition Condition;
typedef Condition * ConditionPtr;

typedef struct Action Action;
typedef Action * ActionPtr;

typedef struct Scenario Scenario;
typedef Scenario * ScenarioPtr;

#include "Trigger.h"
#include "Condition.h"
#include "Action.h"
#include "ActionQueue.h"
#include "EntityGetter.h"
#include "String.h"
#include "PlaneGetter.h"
#include "../objects/IoHandler.h"

struct Trigger {
    TriggerType type;
    void * particulars;
};

struct Listener {
	TriggerType type;
    int priority;
	void * particulars;
};

struct Condition {
	ConditionType type;
	void * particulars;
};

struct Action {
	ActionType type;
	void * particulars;
	bool is_async;
};

struct Scenario {
	ListenerPtr * listeners;
	ConditionPtr * conditions;
	ActionPtr * actions;
	NumericPtr * numerics;
	EntityGetterPtr * entity_getters;
	StringPtr * strings;
	PlaneGetterPtr * plane_getters;
	ActionQueuePtr * action_queues;

	size_t no_listeners;
	size_t no_conditions;
	size_t no_actions;
	size_t no_numerics;
	size_t no_entity_getters;
	size_t no_strings;
	size_t no_plane_getters;
	size_t no_action_queues;

	bool active;
};

struct ScenarioManager {
	ScenarioPtr * scenarios;
	bool * scenario_enabled;
	size_t no_scenarios;

	TriggerPtr * triggers;
	size_t no_triggers;
};

void Trigger_init(TriggerPtr me, TriggerType type, va_list * args);
void Trigger_delete(TriggerPtr me);

void Listener_init(
	ListenerPtr me, IoObject * base, IoHandler * io_handler, GamePtr game,
	int priority
);
bool Listener_catch_trigger(ListenerPtr me, TriggerPtr trigger);

void Condition_init(
	ConditionPtr me, IoObject * base, IoHandler * io_handler, GamePtr game
);
bool Condition_check(ConditionPtr me);

void Action_init(
	ActionPtr me, IoObject * base, IoHandler * io_handler, GamePtr game
);
void Action_make_async(ActionPtr me);
bool Action_run(ActionPtr me);

void Scenario_init(
	ScenarioPtr me, size_t no_listeners, size_t no_conditions,
	size_t no_actions
);
ActionQueuePtr Scenario_add_action_queue(ScenarioPtr me);
void Scenario_check_conditions(ScenarioPtr me);

void ScenarioManager_init(ScenarioManagerPtr me);
ScenarioPtr ScenarioManager_add_scenario(
	ScenarioManagerPtr me, size_t no_listeners, size_t no_conditions,
	size_t no_actions
);
TriggerPtr ScenarioManager_queue_trigger(
	ScenarioManagerPtr me, TriggerType type, size_t no_args, ...
);
void ScenarioManager_handle_scenarios(ScenarioManagerPtr me);
void ScenarioManager_clean_triggers(ScenarioManagerPtr me);
void ScenarioManager_run_action_queues(ScenarioManagerPtr me);

#endif

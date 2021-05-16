/**
 * Scribbler
 * Copyright (C) 2021 Elliot Paton-Simpson
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

#include "Trigger.h"

typedef enum {
	LAST_DIALOGUE_OPTION
} ConditionType;

typedef enum {
	ACTION_SPEAK,
	ACTION_DIALOGUE_OPTIONS
} ActionType;

struct Trigger {
    TriggerType type;
    void * particulars;
};

typedef struct {
	TriggerType type;
    int priority;
	void * particulars;
} Listener;

typedef Listener * ListenerPtr;

typedef struct {
	ConditionType type;
} Condition;

typedef Condition * ConditionPtr;

typedef struct {
	ActionType type;
} Action;

typedef Action * ActionPtr;

typedef struct {
	ListenerPtr * listeners;
	ConditionPtr * conditions;
	ActionPtr * actions;

	size_t no_listeners;
	size_t no_conditions;
	size_t no_actions;

	bool active;
} Scenario;

typedef Scenario * ScenarioPtr;

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
	ListenerPtr me, TriggerType type, int priority, va_list * args
);
bool Listener_catch_trigger(ListenerPtr me, TriggerPtr trigger);

void Condition_init(ConditionPtr me, ConditionType type);

void Action_init(ActionPtr me, ActionType type);

void Scenario_init(ScenarioPtr me);
ListenerPtr Scenario_add_listener(
	ScenarioPtr me, TriggerType type, int priority, size_t no_args, ...
);
void Scenario_check_conditions(ScenarioPtr me);

void ScenarioManager_init(ScenarioManagerPtr me);
ScenarioPtr ScenarioManager_add_scenario(ScenarioManagerPtr me);
TriggerPtr ScenarioManager_queue_trigger(
	ScenarioManagerPtr me, TriggerType type, size_t no_args, ...
);
void ScenarioManager_handle_scenarios(ScenarioManagerPtr me);
void ScenarioManager_clean_triggers(ScenarioManagerPtr me);

#endif

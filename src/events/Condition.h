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

#ifndef __CONDITION_H__
#define __CONDITION_H__

#include <stdarg.h>
#include <stddef.h>

typedef enum {
	NUMERIC_LESS_THAN,
	NUMERIC_LESS_THAN_OR_EQUAL_TO,
	NUMERIC_GREATER_THAN,
	NUMERIC_GREATER_THAN_OR_EQUAL_TO,
	NUMERIC_EQUAL_TO,
	MAX_CONDITIONS
} ConditionType;

extern size_t condition_sizes[MAX_CONDITIONS];

void pre_init_condition_functions();
void pre_init_condition_sizes();
void pre_init_condition_evaluate();
void init_conditions();

#include "conditions/NumericLessThanCondition.h"
#include "conditions/NumericLessThanOrEqualToCondition.h"
#include "conditions/NumericGreaterThanOrEqualToCondition.h"
#include "conditions/NumericGreaterThanCondition.h"
#include "conditions/NumericEqualToCondition.h"

extern void (*init_for_condition_functions[MAX_CONDITIONS]) (
	void * me_void, va_list * args
);

extern bool (*evaluate_for_condition_functions[MAX_CONDITIONS]) (
	void * me_void
);


#endif

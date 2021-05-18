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

#ifndef __NUMERIC_H__
#define __NUMERIC_H__

#include <stdarg.h>
#include <stddef.h>

typedef enum {
	RAW_NUMERIC,
	ENTITY_GET_X,
	ENTITY_GET_Y,
	MAX_NUMERICS
} NumericType;

extern size_t numeric_sizes[MAX_NUMERICS];

typedef struct {
	NumericType type;
	void * particulars;
} Numeric;

typedef Numeric * NumericPtr;

void Numeric_init(NumericPtr me, NumericType type, va_list * args);
double Numeric_evaluate(NumericPtr me);

void pre_init_numeric_functions();
void pre_init_numeric_sizes();
void pre_init_numeric_evaluate();
void init_numerics();

#include "numeric/EntityGetX.h"
#include "numeric/EntityGetY.h"
#include "numeric/RawNumeric.h"

extern void (*init_for_numeric_functions[MAX_NUMERICS]) (
	void * me_void, va_list * args
);

extern double (*evaulate_for_numeric_functions[MAX_NUMERICS]) (void * me_void);

#endif

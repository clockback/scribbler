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

#ifndef __STRING_H__
#define __STRING_H__

#include <stdarg.h>
#include <stddef.h>

typedef struct String String;
typedef String * StringPtr;

typedef enum {
	RAW_STRING,
	MAX_STRINGS
} StringType;

#include "../ECS/ECS.h"

extern size_t string_sizes[MAX_STRINGS];

struct String {
	StringType type;
	void * particulars;
};

void String_init(
	StringPtr me, StringType type, va_list * args
);
char * String_evaluate(StringPtr me);

void pre_init_string_functions();
void pre_init_string_sizes();
void pre_init_string_evaluate();
void init_strings();

#include "strings/RawStringString.h"

extern void (*init_for_string_functions[MAX_STRINGS]) (
	void * me_void, va_list * args
);

extern char * (*evaulate_for_string_functions[MAX_STRINGS]) (
	void * me_void
);

#endif

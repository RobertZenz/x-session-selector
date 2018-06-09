/*
 * Copyright 2017, Robert 'Bobby' Zenz
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef UTIL_C
#define UTIL_C

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


bool endswith(const char* value, const char* postfix) {
	if (!value || !postfix) {
		return false;
	}
	
	size_t value_length = strlen(value);
	size_t postfix_length = strlen(postfix);
	
	if (value_length < postfix_length) {
		return false;
	}
	
	return strncmp(value + value_length - postfix_length, postfix, postfix_length) == 0;
}

char* extract(const char* value, int count) {
	if (!value) {
		return NULL;
	}
	
	size_t length = strlen(value);
	
	if (count >= length) {
		return NULL;
	}
	
	if (endswith(value, "\n")) {
		length--;
	}
	
	char* clone = malloc(sizeof(char) * (length - count + 1));
	
	strncpy(clone, value + count, length - count);
	clone[length - count] = 0;
	
	return clone;
}

void make_path(char* target, char* a, char* b) {
	target[0] = 0;
	
	strncat(target, a, strlen(a));
	
	if (!endswith(a, "/")) {
		strncat(target, "/", strlen("/"));
	}
	
	strncat(target, b, strlen(b));
}

bool startswith(const char* value, const char* prefix) {
	if (!value || !prefix) {
		return false;
	}
	
	size_t value_length = strlen(value);
	size_t prefix_length = strlen(prefix);
	
	if (value_length < prefix_length) {
		return false;
	}
	
	return strncmp(value, prefix, prefix_length) == 0;
}

char** split(char* value, int* size, int offset) {
	// Trailing NULL and assume at least one item.
	*size = *size + 2;
	
	char** splitted = malloc(sizeof(char*) * *size);
	
	char* beginning = value;
	bool quoted = false;
	int index = offset;
	
	while (value[0]) {
		if (value[0] == '"') {
			quoted = !quoted;
		} else if (!quoted && value[0] == ' ') {
			*size = *size + 1;
			splitted = realloc(splitted, sizeof(char*) * *size);
			
			value[0] = '\0';
			splitted[index++] = beginning;
			beginning = value + 1;
		}
		
		value++;
	}
	splitted[index++] = beginning;
	
	while (index < *size) {
		splitted[index++] = NULL;
	}
	
	return splitted;
}

#endif


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

#ifndef TTY_C
#define TTY_C

#include <string.h>
#include <unistd.h>

#include "util.c"


#define NO_TTY -1


char* append_to_vt_char(char*);
char* append_to_vt_int(int);
int get_current_vt_number();
char* get_current_vt();


char* append_to_vt_char(char* value) {
	int number = 0;
	
	if (value[0] == '+' || value[0] == '-') {
		int currentvt = get_current_vt_number();
		
		if (currentvt <= 0) {
			return NULL;
		}
		
		number = get_current_vt_number() + atoi(value);
	} else {
		number = atoi(value);
	}
	
	return append_to_vt_int(number);
}

char* append_to_vt_int(int number) {
	if (number <= 0) {
		return NULL;
	}
	
	char buffer[9];
	int length = snprintf(buffer, 9, "%d", number);
	
	char* vt = malloc(sizeof(char) * (2 + length + 1));
	vt[0] = 'v';
	vt[1] = 't';
	strncpy(vt + 2, buffer, length);
	vt[2 + length] = '\0';
	
	return vt;
}

int get_current_vt_number() {
	char* tty = ttyname(STDIN_FILENO);
	
	if (startswith(tty, "/dev/tty")) {
		return atoi(tty + 8);
	}
	
	return NO_TTY;
}

char* get_current_vt() {
	return append_to_vt_int(get_current_vt_number());
}

#endif


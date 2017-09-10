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

char* append_to_vt_char(char* number, int offset, int length) {
	char* vt = malloc(sizeof(char) * (2 + length + 1));
	vt[0] = 'v';
	vt[1] = 't';
	strncpy(vt + 2, number + offset, length);
	vt[2 + length] = '\0';
	
	return vt;
}

char* append_to_vt_int(int number) {
	char buffer[9];
	int length = snprintf(buffer, 9, "%d", number);
	
	return append_to_vt_char(buffer, 0, length);
}

char* get_current_vt() {
	char* tty = ttyname(STDIN_FILENO);
	
	if (startswith(tty, "/dev/tty")) {
		size_t ttylength = strlen(tty);
		int numberlength = ttylength - 8;
		
		return append_to_vt_char(tty, 8, numberlength);
	}
	
	return NULL;
}

#endif


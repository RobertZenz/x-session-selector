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

#ifndef X11_C
#define X11_C

#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/param.h>
#include <unistd.h>

#include "util.c"


char* append_to_colon_int(int number) {
	char* display = malloc(sizeof(char) * (1 + 8));
	snprintf(display, sizeof(char) * (1 + 8), ":%d", number);
	
	return display;
}

char* get_next_free_display() {
	DIR* dir = opendir("/tmp/.X11-unix/");
	
	if (dir) {
		int number = 0;
		bool addone = false;
		
		struct dirent* entry = NULL;
		
		while ((entry = readdir(dir)) != NULL) {
			if (startswith(entry->d_name, "X")) {
				addone = true;
				number = MAX(number, atoi(entry->d_name + 1));
			}
		}
		
		if (addone) {
			number++;
		}
		
		return append_to_colon_int(number);
	}
	
	return ":0";
}

#endif


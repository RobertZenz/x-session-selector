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

#ifndef CONFIGURATION_C
#define CONFIGURATION_C

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "tty.c"
#include "x11.c"


struct configuration {
	bool automatic;
	char* display;
	bool nonewline;
	bool printonly;
	char* selection;
	int timeout;
	char* vt;
	char* xsessionsdir;
};


struct configuration configure(int argcount, char** args) {
	struct configuration config;
	config.automatic = false;
	config.display = get_next_free_display();
	config.nonewline = false;
	config.printonly = false;
	config.selection = NULL;
	config.timeout = 2;
	config.vt = get_current_vt();
	config.xsessionsdir = "/usr/share/xsessions/";
	
	int c;
	
	while((c = getopt(argcount, args, "a::d:nos:t:v:x:")) != -1) {
		switch (c) {
			case 'a':
				config.automatic = true;
				
				if (optarg) {
					config.timeout = atoi(optarg);
				}
				break;
				
			case 'd':
				if (optarg[0] == ':') {
					config.display = optarg;
				}
				else {
					config.display = append_to_colon_int(atoi(optarg));
				}
				break;
				
			case 'n':
				config.nonewline = true;
				break;
				
			case 'o':
				config.printonly = true;
				break;
				
			case 's':
				config.selection = optarg;
				break;
				
			case 't':
				config.timeout = atoi(optarg);
				break;
				
			case 'v':
				config.vt = append_to_vt_int(atoi(optarg));
				break;
				
			case 'x':
				config.xsessionsdir = optarg;
				break;
				
		}
	}
	
	return config;
}

#endif


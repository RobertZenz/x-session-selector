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


struct configuration {
	bool automatic;
	bool nonewline;
	bool printonly;
	char* selection;
};


struct configuration configure(int argcount, char** args) {
	struct configuration config;
	config.automatic = false;
	config.nonewline = false;
	config.printonly = false;
	config.selection = NULL;
	
	int c;
	
	while((c = getopt(argcount, args, "anos:")) != -1) {
		switch (c) {
			case 'a':
				config.automatic = true;
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
			
		}
	}
	
	return config;
}

#endif


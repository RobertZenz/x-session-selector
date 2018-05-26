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

#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "tty.c"
#include "x11.c"


struct configuration {
	bool automatic;
	char* display;
	bool error;
	bool help;
	bool nonewline;
	bool printonly;
	char* selection;
	int timeout;
	char* vt;
	char* xlauncher;
	char* xsessionsdir;
};


static struct option options[] = {
	{ "automatic", optional_argument, 0, 0 },
	{ "display", required_argument, 0, 0 },
	{ "help", no_argument, 0, 0 },
	{ "no-newline", no_argument, 0, 0 },
	{ "print-only", no_argument, 0, 0 },
	{ "select", required_argument, 0, 0 },
	{ "timeout", required_argument, 0, 0 },
	{ "vt", required_argument, 0, 0 },
	{ "xlauncher", required_argument, 0, 0 },
	{ "xsessions-dir", required_argument, 0, 0 },
	{ NULL, 0, NULL, 0 }
};


struct configuration configure(int argcount, char** args) {
	struct configuration config;
	config.automatic = false;
	config.display = get_next_free_display();
	config.error = false;
	config.help = false;
	config.nonewline = false;
	config.printonly = false;
	config.selection = NULL;
	config.timeout = 2;
	config.vt = get_current_vt();
	config.xlauncher = "xinit";
	config.xsessionsdir = "/usr/share/xsessions/";
	
	int value = 0;
	int optionindex = 0;
	
	while((value = getopt_long(
					   argcount,
					   args,
					   "",
					   options,
					   &optionindex)) != -1) {
		if (value == 0) {
			struct option option = options[optionindex];
			
			if (strcmp(option.name, "automatic") == 0) {
				config.automatic = true;
				
				if (optarg) {
					config.timeout = atoi(optarg);
				}
			} else if (strcmp(option.name, "display") == 0) {
				if (optarg[0] == ':') {
					config.display = optarg;
				} else {
					config.display = append_to_colon_int(atoi(optarg));
				}
			} else if (strcmp(option.name, "help") == 0) {
				config.help = true;
			} else if (strcmp(option.name, "no-newline") == 0) {
				config.nonewline = true;
			} else if (strcmp(option.name, "print-only") == 0) {
				config.printonly = true;
			} else if (strcmp(option.name, "select") == 0) {
				config.selection = optarg;
			} else if (strcmp(option.name, "timeout") == 0) {
				config.timeout = atoi(optarg);
			} else if (strcmp(option.name, "vt") == 0) {
				config.vt = append_to_vt_char(optarg);
			} else if (strcmp(option.name, "xlauncher") == 0) {
				config.xlauncher = optarg;
			} else if (strcmp(option.name, "xsessions-dir") == 0) {
				config.xsessionsdir = optarg;
			}
		} else if (value == '?') {
			config.error = true;
		}
	}
	
	return config;
}

#endif


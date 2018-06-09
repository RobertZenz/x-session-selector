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

#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#include "configuration.c"
#include "list.c"
#include "sessions.c"
#include "tui.c"
#include "windowmanagers.c"


int main(int argc, char** argv) {
	struct configuration config = configure(argc, argv);
	
	if (config.error) {
		fprintf(stderr, "Try 'xsessionselector --help' for more information.\n");
		
		return EXIT_FAILURE;
	}
	
	if (config.help) {
		printf(
			"USAGE: xsessionselector [OPTION]\n"
			"\n"
			"  --automatic[=TIMEOUT]  Activates the automatic selection system, if no\n"
			"                         timeout is provided it defaults to 2 seconds.\n"
			"  --display=DISPAY       The display number to assign. If not provided will\n"
			"                         default to the next free X display number.\n"
			"  --help                 Prints this help.\n"
			"  --no-newline           When --print-only is provided, does not print\n"
			"                         a new line after the selection.\n"
			"  --print-only           Only print the selection to stdout (if any).\n"
			"  --select=SELECTION     Select this item, the provided argument is\n"
			"                         the executable, f.e. /usr/bin/openbox.\n"
			"  --shell=SHELL          The executable to start when dropping to shell,\n"
			"                         defaults to nothing. If this is not provided,\n"
			"                         the program will simply exit, if a value is\n"
			"                         provided, it will be executed.\n"
			"  --timeout=TIMEOUT      When --automatic is provided, sets the timeout,\n"
			"                         defaults to 2 seconds.\n"
			"  --tty-only             Only present the prompt when being on a TTY.\n"
			"                         When not on a TTY the program will exit, except\n"
			"                         if --shell has been provided, then the shell will\n"
			"                         be executed.\n"
			"  --vt=VT                The number of the virtual terminal to use,\n"
			"                         defaults to the current VT. Provided number must\n"
			"                         be positive, but can be prefixed with + or - to\n"
			"                         to change relative from the current VT.\n"
			"  --wsessions-dir=DIR    The directory from which to read the available\n"
			"                         Wayland sessions, defaults to\n"
			"                         /usr/share/wayland-sessions/.\n"
			"  --xlauncher=XLAUNCHER  The executable to use as launcher for X, defaults\n"
			"                         to xinit. The selected session together with\n"
			"                         the display number will be given as arguments to\n"
			"                         the provided executable.\n"
			"  --xsessions-dir=DIR    The directory from which to read the available\n"
			"                         X sessions, defaults to /usr/share/xsessions/.\n");
			
		return EXIT_SUCCESS;
	}
	
	if (config.ttyonly && get_current_vt_number() == NO_TTY) {
		if (config.shell) {
			execlp(
				config.shell,
				config.shell,
				(char*)NULL);
				
			return errno;
		} else {
			return EXIT_SUCCESS;
		}
	}
	
	struct list* lists[3];
	lists[0] = read_sessions(config.xsessionsdir, X);
	lists[1] = read_sessions(config.wsessionsdir, WAYLAND);
	lists[2] = get_windowmanagers();
	
	struct item* startitem = userselect(
								 lists,
								 3,
								 find_item(lists, 3, config.selection),
								 config.automatic,
								 config.timeout);
								 
	if (startitem) {
		if (config.printonly) {
			printf("%s", startitem->exec);
			
			if (!config.nonewline) {
				printf("\n");
			}
		} else {
			int size = 0;
			char** command = NULL;
			
			if (startitem->type == X) {
				size = 4;
				command = split(startitem->exec, &size, 1);
				command[0] = config.xlauncher;
				command[size - 4] = "--";
				command[size - 3] = config.display;
				command[size - 2] = config.vt;
			} else if (startitem->type == WAYLAND) {
				command = split(startitem->exec, &size, 0);
			}
			
			execvp(
				command[0],
				command);
				
			return errno;
		}
	} else if (config.shell) {
		execlp(
			config.shell,
			config.shell,
			(char*)NULL);
			
		return errno;
	}
	
	return EXIT_SUCCESS;
}


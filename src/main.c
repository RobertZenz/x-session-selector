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
			"  --timeout=TIMEOUT      When --automatic is provided, sets the timeout,\n"
			"                         defaults to 2 seconds.\n"
			"  --vt=VT                The number of the virtual terminal to use.\n"
			"  --xlauncher=XLAUNCHER  The executable to use as launcher for X, defaults\n"
			"                         to xinit. The selected session together with\n"
			"                         the display number will be given as arguments to\n"
			"                         the provided executable.\n"
			"  --xsessions-dir=DIR    The directory from which to read the available\n"
			"                         X sessions, defaults to /usr/share/xsessions/.\n");
			
		return EXIT_SUCCESS;
	}
	
	struct list* lists[2];
	lists[0] = read_sessions(config.xsessionsdir);
	lists[1] = get_windowmanagers();
	
	struct item* startitem = userselect(
								 lists,
								 2,
								 find_item(lists, 2, config.selection),
								 config.automatic,
								 config.timeout);
								 
	if (startitem) {
		if (config.printonly) {
			printf("%s", startitem->exec);
			
			if (!config.nonewline) {
				printf("\n");
			}
		} else {
			return execlp(
					   config.xlauncher,
					   config.xlauncher,
					   startitem->exec,
					   "--",
					   config.display,
					   config.vt,
					   (char*)NULL);
		}
	}
	
	return EXIT_SUCCESS;
}


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

#include <stdlib.h>

#include "configuration.c"
#include "list.c"
#include "sessions.c"
#include "tui.c"
#include "tty.c"
#include "windowmanagers.c"
#include "x11.c"


int main(int argc, char** argv) {
	struct configuration config = configure(argc, argv);
	
	struct list* lists[2];
	lists[0] = read_sessions("/usr/share/xsessions/");
	lists[1] = get_windowmanagers();
	
	struct item* startitem = userselect(lists, 2, config.selection);
	
	if (startitem) {
		if (config.printonly) {
			printf("%s", startitem->exec);
			
			if (!config.nonewline) {
				printf("\n");
			}
		} else {
			return execlp(
					"xinit",
					"xinit",
					startitem->exec,
					"--",
					get_next_display(),
					get_current_vt(),
					(char*)NULL);
		}
	}
	
	return EXIT_SUCCESS;
}


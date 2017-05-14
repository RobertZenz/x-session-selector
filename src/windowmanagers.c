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

#ifndef WINDOWMANAGERS_C
#define WINDOWMANAGERS_C

#include <stdlib.h>
#include <unistd.h>

#include "configuration.c"
#include "list.c"
#include "sessions.c"
#include "tui.c"
#include "util.c"


void add_windowmanager(struct list* windowmanagers, char* name, char* exec) {
	add_to_list(windowmanagers, new_item(strdup(name), strdup(exec)));
}

struct list* get_available(struct list* programs) {
	char* path = strdup(getenv("PATH"));
	char* piece = NULL;
	
	struct list* availables = new_list(programs->name);
	
	struct item* program = NULL;
	char* programpath = malloc(sizeof(char) * (PATH_MAX + 1));
	
	piece = strtok(path, ":");
	while(piece) {
		int index = 0;
		for (index = 0; index < programs->length; index++) {
			program = programs->items[index];
			
			make_path(programpath, piece, program->exec);
			
			if (access(programpath, F_OK | X_OK) != -1) {
				add_to_list(availables, new_item(strdup(program->name), strdup(programpath)));
				free_item(remove_from_list(programs, index));
				
				index--;
			}
			
			program = NULL;
		}
		
		piece = strtok(NULL, ":");
	}
	
	free(programpath);
	free(path);
	
	sort_list(availables);
	
	return availables;
}

struct list* get_windowmanagers(void) {
	struct list* windowmanagers = new_list(strdup("Window managers"));
	// This list is copied from Wikipedia.
	// https://en.wikipedia.org/wiki/Comparison_of_X_window_managers
	add_windowmanager(windowmanagers, "9wm", "9wm");
	add_windowmanager(windowmanagers, "aewm", "aewm");
	add_windowmanager(windowmanagers, "awesome", "awesome");
	add_windowmanager(windowmanagers, "Blackbox", "blackbox");
	add_windowmanager(windowmanagers, "Compiz", "compiz");
	add_windowmanager(windowmanagers, "cwm", "cwm");
	add_windowmanager(windowmanagers, "dwm", "dwm");
	add_windowmanager(windowmanagers, "Enlightenment", "???");
	add_windowmanager(windowmanagers, "evilwm", "evilwm");
	add_windowmanager(windowmanagers, "Fluxbox", "fluxbox");
	add_windowmanager(windowmanagers, "FLWM", "flwm");
	add_windowmanager(windowmanagers, "FVWM", "fvwm");
	add_windowmanager(windowmanagers, "herbstluftwm", "herbstluftwm");
	add_windowmanager(windowmanagers, "i3", "i3");
	add_windowmanager(windowmanagers, "IceWM", "icewm");
	add_windowmanager(windowmanagers, "Ion", "ion");
	add_windowmanager(windowmanagers, "JWM", "jwm");
	add_windowmanager(windowmanagers, "KWin (KDE)", "kwin");
	add_windowmanager(windowmanagers, "Marco (MATE)", "marco");
	add_windowmanager(windowmanagers, "Matchbox", "matchbox");
	add_windowmanager(windowmanagers, "Metacity (GNOME)", "metacity");
	add_windowmanager(windowmanagers, "Mutter (GNOME/MeeGo)", "mutter");
	add_windowmanager(windowmanagers, "mwm (Motif Window Manager)", "mwm");
	add_windowmanager(windowmanagers, "Openbox", "openbox");
	add_windowmanager(windowmanagers, "PekWM", "pekwm");
	add_windowmanager(windowmanagers, "PlayWM", "playwm");
	add_windowmanager(windowmanagers, "Ratpoison", "ratpoison");
	add_windowmanager(windowmanagers, "Sawfish", "sawfish");
	add_windowmanager(windowmanagers, "sithWM", "sithwm");
	add_windowmanager(windowmanagers, "spectrwm", "spectrwm");
	add_windowmanager(windowmanagers, "steamcompmgr", "steamcompmgr");
	add_windowmanager(windowmanagers, "StumpWM", "stumpwm");
	add_windowmanager(windowmanagers, "twm", "twm");
	add_windowmanager(windowmanagers, "WMFS", "wmfs");
	add_windowmanager(windowmanagers, "Window Maker", "wmaker");
	add_windowmanager(windowmanagers, "Wingo", "wingo");
	add_windowmanager(windowmanagers, "wmii", "wmii");
	add_windowmanager(windowmanagers, "Xfwm (Xfce)", "xfwm");
	add_windowmanager(windowmanagers, "xmonad", "xmonad");
	add_windowmanager(windowmanagers, "uwm", "uwm");
	
	struct list* availablewindowmanagers = get_available(windowmanagers);
	
	free(windowmanagers);
	
	sort_list(availablewindowmanagers);
	
	return availablewindowmanagers;
}

#endif


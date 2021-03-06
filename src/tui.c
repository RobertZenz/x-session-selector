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

#ifndef TUI_C
#define TUI_C

#include <ncurses.h>
#include <stdbool.h>


#define KEY_ESC 27
#define KEY_ENT 10

#define COLORS_BACKGROUND 1
#define COLORS_DEFAULT 2
#define COLORS_HIGHLIGHT 3


int count_lists(struct list* lists[], int listscount) {
	int count = 0;
	
	int index = 0;
	for (index = 0; index < listscount; index++) {
		if (lists[index] && lists[index]->length > 0) {
			count++;
		}
	}
	
	return count;
}

void draw_separator_line(WINDOW* window, int y, int x, int width) {
	mvwaddch(window, y, x, ACS_LTEE);
	mvwhline(window, y, x + 1, 0, width - 2);
	mvwaddch(window, y, x + width - 1, ACS_RTEE);
}

void draw_button(WINDOW* window, int y, int x, int highlight, const char* text) {
	size_t length = strlen(text);
	
	mvwaddch(window, y, x, ACS_RTEE);
	
	if (highlight) {
		wattron(window, COLOR_PAIR(COLORS_HIGHLIGHT));
	} else {
		wattron(window, COLOR_PAIR(COLORS_DEFAULT));
	}
	mvwprintw(window, y, x + 1, text);
	
	wattron(window, COLOR_PAIR(COLORS_DEFAULT));
	mvwaddch(window, y, x + 1 + length, ACS_LTEE);
}

void draw_item(WINDOW* window, int y, int x, int width, int selected, struct item* item) {
	if (selected) {
		wattron(window, COLOR_PAIR(COLORS_HIGHLIGHT));
	} else {
		wattron(window, COLOR_PAIR(COLORS_DEFAULT));
	}
	
	mvwprintw(window, y, x, "%s ", item->name);
	
	size_t right = strlen(item->exec);
	
	int fillerx = 0;
	for (fillerx = 1 + strlen(item->name) + 2; fillerx < width - right; fillerx++) {
		mvwprintw(window, y, fillerx, ".");
	}
	
	mvwprintw(window, y, width - right, " %s", item->exec);
	
	wattron(window, COLOR_PAIR(COLORS_DEFAULT));
}

void draw_list(WINDOW* window, int y, int x, int width, int selectedindex, struct list* list) {
	wattron(window, COLOR_PAIR(COLORS_DEFAULT));
	
	if (y > 0) {
		draw_separator_line(window, y, 0, width);
	}
	
	mvwprintw(window, y, 3, " %s ", list->name);
	
	int index = 0;
	for (index = 0; index < list->length; index++) {
		draw_item(window, y + 2 + index, x + 2, width - 3, selectedindex == index, list->items[index]);
	}
}

void draw_lists(WINDOW* window, int y, int x, int width, int selectedindex, struct list* lists[], int listscount) {
	int yoffset = 0;
	
	int listoffset = 0;
	
	int index = 0;
	for (index = 0; index < listscount; index++) {
		if (lists[index] && lists[index]->length > 0) {
			draw_list(
				window,
				y + yoffset + (listoffset * 3),
				x,
				width,
				selectedindex - yoffset,
				lists[index]);
			yoffset = yoffset + lists[index]->length;
			listoffset++;
		}
	}
}

int find_index(struct list* lists[], int listscount, struct item* needle) {
	int item_offset = 0;
	
	struct list* list = NULL;
	
	int index = 0;
	for (index = 0; index < listscount; index++) {
		list = lists[index];
		
		if (list) {
			struct item* item = NULL;
			
			int item_index = 0;
			for (item_index = 0; item_index < list->length; item_index++) {
				item = list->items[item_index];
				
				if (item && strcmp(item->exec, needle->exec) == 0) {
					return item_offset + item_index;
				}
			}
			
			item_offset = item_offset + list->length;
		}
	}
	
	return -1;
}

struct list* merge(struct list* lists[], int listscount) {
	struct list* combined = new_list(NULL);
	
	struct list* list = NULL;
	
	int index = 0;
	for (index = 0; index < listscount; index++) {
		list = lists[index];
		
		if (list) {
			int item_index = 0;
			for (item_index = 0; item_index < list->length; item_index++) {
				add_to_list(combined, list->items[item_index]);
			}
		}
	}
	
	return combined;
}

void setup_ncurses()
{
	initscr();
	noecho();
	curs_set(false);
	
	start_color();
	use_default_colors();
	
	init_pair(COLORS_BACKGROUND, -1, -1);
	init_pair(COLORS_DEFAULT, -1, -1);
	init_pair(COLORS_HIGHLIGHT, COLOR_BLACK, COLOR_WHITE);
	
	bkgd(COLOR_PAIR(COLORS_BACKGROUND));
}

struct item* userselect(struct list* lists[], int listscount, struct item* selection, bool automatic, int automatictimeout) {
	setup_ncurses();
	
	struct list* allitems = merge(lists, listscount);
	
	int reallistscount = count_lists(lists, listscount);
	
	int height = allitems->length + (reallistscount * 3) + 1;
	int width = 74;
	int winy = (LINES - height) / 2;
	int winx = (COLS - width) / 2;
	
	WINDOW* window = newwin(height, width, winy, winx);
	keypad(window, true);
	wbkgd(window, COLOR_PAIR(COLORS_DEFAULT));
	
	bool run = true;
	int key = 0;
	bool ok = true;
	int selected = 0;
	struct item* selecteditem = NULL;
	
	if (selection) {
		selected = find_index(lists, listscount, selection);
	}
	
	bool automaticactive = automatic;
	int automaticcountdown = automatictimeout * 60;
	
	size_t automessagebuffersize = 128;
	char automessagebuffer[automessagebuffersize];
	int automessagelength = 0;
	
	while (run) {
		wattron(window, COLOR_PAIR(COLORS_DEFAULT));
		
		if (automaticactive) {
			wtimeout(window, 16);
			
			automessagelength = snprintf(
									automessagebuffer,
									automessagebuffersize,
									" Starting in %d seconds...",
									automaticcountdown / 60 + 1);
		} else {
			wtimeout(window, -1);
		}
		
		box(window, 0, 0);
		
		draw_lists(window, 0, 0, width, ok ? selected : -1, lists, listscount);
		
		draw_button(window, height - 1, 3, !ok, " Drop to shell ");
		
		if (automaticactive) {
			draw_button(window, height - 1, width - automessagelength - 5, ok, automessagebuffer);
		} else {
			draw_button(window, height - 1, width - 9, ok, " OK ");
		}
		
		/* Set the cursor to 0, 0 to make sure that pressed keys are not
		 * printed. */
		wmove(window, 0, 0);
		
		refresh();
		wrefresh(window);
		
		key = wgetch(window);
		
		switch (key) {
			case 'q':
				run = false;
				break;
				
			case KEY_ESC:
				if (automaticactive) {
					automaticactive = false;
				} else {
					run = false;
				}
				break;
				
			case KEY_ENT:
				if (ok) {
					selecteditem = allitems->items[selected];
				}
				run = false;
				break;
				
			case KEY_LEFT:
				ok = false;
				break;
				
			case KEY_RIGHT:
				ok = true;
				break;
				
			case KEY_UP:
				if (ok && selected > 0) {
					selected--;
				}
				ok = true;
				break;
				
			case KEY_DOWN:
				if (ok && selected < (allitems->length - 1)) {
					selected++;
				}
				ok = true;
				break;
				
			case ERR:
				// Timeout encountered.
				automaticcountdown--;
				
				if (automaticcountdown <= 0) {
					run = false;
				}
				break;
				
			default:
				// Ignore anything else.
				break;
		}
		
		if (automaticactive && key != ERR) {
			automaticactive = false;
		}
	}
	
	endwin();
	
	return selecteditem;
}

#endif


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

#ifndef LIST_C
#define LIST_C


struct item {
	char* name;
	char* exec;
};

struct list {
	struct item** items;
	int length;
	char* name;
};


void add_to_list(struct list* list, struct item* item) {
	if (list && item) {
		if (list->items == NULL) {
			list->items = malloc(sizeof(struct item*));
		} else {
			list->items = realloc(list->items, (list->length + 1) * sizeof(struct item*));
		}
		
		list->items[list->length] = item;
		list->length++;
	}
}

int compare_item(const void* a, const void* b) {
	struct item* itema = *(struct item**)a;
	struct item* itemb = *(struct item**)b;
	
	if (!itema && !itemb) {
		return 0;
	} else if (!itema) {
		return -1;
	} else if (!itemb) {
		return 1;
	}
	
	return strcmp(itema->name, itemb->name);
}

struct item* find_item(struct list* lists[], int listscount, char* itemexec) {
	if (!itemexec) {
		return NULL;
	}
	
	struct list* list = NULL;
	
	int index = 0;
	for (index = 0; index < listscount; index++) {
		list = lists[index];
		
		if (list) {
			struct item* item = NULL;
			
			int item_index = 0;
			for (item_index = 0; item_index < list->length; item_index++) {
				item = list->items[item_index];
				
				if (item && strcmp(item->exec, itemexec) == 0) {
					return item;
				}
			}
		}
	}
	
	return NULL;
}

void free_item(struct item* item) {
	if (item) {
		if (item->name) {
			free(item->name);
		}
		
		if (item->exec) {
			free(item->exec);
		}
		
		free(item);
	}
}

void free_list(struct list* list) {
	if (list) {
		int index = 0;
		
		for (index = 0; index < list->length; index++) {
			free_item(list->items[index]);
		}
		
		free(list);
	}
}

struct item* new_item(char* name, char* exec) {
	struct item* item = malloc(sizeof(struct item));
	item->name = name;
	item->exec = exec;
	
	return item;
}

struct list* new_list(char* name) {
	struct list* list = malloc(sizeof(struct list));
	list->items = NULL;
	list->length = 0;
	list->name = name;
	
	return list;
}

struct item* remove_from_list(struct list* list, int remove_index) {
	if (remove_index >= 0 && remove_index < list->length) {
		struct item* removeditem = list->items[remove_index];
		
		int index = 0;
		for (index = remove_index; index < list->length - 1; index++) {
			list->items[index] = list->items[index + 1];
		}
		
		list->items[list->length - 1] = NULL;
		
		list->items = realloc(list->items, (list->length - 1) * sizeof(struct item*));
		list->length--;
		
		return removeditem;
	}
	
	return NULL;
}

void sort_list(struct list* list) {
	qsort(list->items, list->length, sizeof(struct item*), compare_item);
}

#endif


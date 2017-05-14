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

#ifndef SESSIONS_C
#define SESSIONS_C

#include <dirent.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "list.c"
#include "util.c"


bool is_session_file(struct dirent* file) {
	if (!file) {
		return false;
	}
	
	return endswith(file->d_name, ".desktop");
}

struct item* read_session(char *filepath) {
	struct item* session = NULL;
	
	FILE* file = fopen(filepath, "r");
	
	if (file) {
		session = new_item(NULL, NULL);
		
		char* line = NULL;
		ssize_t length = 0;
		
		while (getline(&line, &length, file) > 0) {
			if (startswith(line, "Exec=")) {
				session->exec = extract(line, 5);
			} else if (startswith(line, "Name=")) {
				session->name = extract(line, 5);
			}
		}
		
		if (!session->exec) {
			free_item(session);
			session = NULL;
		} else if (!session->name) {
			session->name = session->exec;
		}
	}
	
	return session;
}

struct list* read_sessions(char* dirpath) {
	DIR* dir = opendir(dirpath);
	
	if (dir) {
		struct list* sessions = new_list("Sessions");
		
		struct dirent* entry = NULL;
		char* filepath = malloc(sizeof(char) * (PATH_MAX + 1));
		
		while ((entry = readdir(dir)) != NULL) {
			if (is_session_file(entry)) {
				make_path(filepath, dirpath, entry->d_name);
				
				add_to_list(sessions, read_session(filepath));
			}
		}
		
		free(filepath);
		
		sort_list(sessions);
		
		return sessions;
	}
	
	return NULL;
}

#endif


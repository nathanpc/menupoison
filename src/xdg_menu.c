/**
 *  xdg_menu.h
 *  XDG menu stuff.
 *
 *  @author Nathan Campos <nathanpc@dreamintech.net>
 */

#include "xdg_menu.h"

void clear_sections_array(struct menuitem item, int n);
char *trim(char *str);
int fnlines(char *command);

int parse_menus(struct menuitem **items) {
	FILE *menufile;
	char line[1024];
	char *command = "update-menus --stdout | grep -i '^command=' | grep -i 'needs=\"x11\"'";
	int lines = fnlines(command);
	struct menuitem *_items = malloc(lines * sizeof(struct menuitem));

	// Execute the command and grab its output.
	menufile = popen(command, "r");
	if (menufile == NULL) {
		printf("Couldn't run the command: %s\n", command);
		exit(EXIT_FAILURE);
	}

	int icount = 0;
	while (fgets(line, sizeof(line) - 1, menufile) != NULL) {
		uint8_t tcount = 1;
		char *ckey;
		char *token;
		char sections[128];

		// Parse the line.
		strtok(line, "\n");  // Remove the trailing newline left by fgets.
		token = strtok(line, "=\"");
		while (token != NULL) {
			if (tcount % 2 ) {
				ckey = trim(token);
			} else {
				if (!strcmp("command", ckey)) {
					strcpy(_items[icount].command, token);
				} else if (!strcmp("section", ckey)) {
					//strcpy(_items[icount].section, token);
					strcpy(sections, token);
					printf("%s\n", sections);
				} else if (!strcmp("title", ckey)) {
					strcpy(_items[icount].title, token);
				}
			}

			token = strtok(NULL, "=\"");
			tcount++;
		}

		// Clear the contents of the sections array.
		clear_sections_array(_items[icount], 5);

		// Split the sections and populate the array.
		tcount = 0;
		token = strtok(sections, "/");
		while ((token != NULL) && (tcount < 5)) {
			strcpy(_items[icount].sections[tcount], token);

			token = strtok(NULL, "/");
			tcount++;
		}

		icount++;
	}

	*items = _items;

	pclose(menufile);
	return lines;
}

/**
 * Counts the number of lines in a command output.
 *
 * @param command Command to be executed by popen.
 * @returns Number of lines.
 */
int fnlines(char *command) {
	FILE *file;
	char c;
	int lines = 0;

	file = popen(command, "r");
	if (file == NULL) {
		printf("Couldn't run the command: %s\n", command);
		exit(EXIT_FAILURE);
	}

	while (!feof(file)) {
		c = fgetc(file);

		if (c == '\n') {
			lines++;
		}
	}

	fclose(file);
	return lines;
}

/**
 * Clears the sections array.
 *
 * @param item Menu item struct.
 * @param n Array size.
 */
void clear_sections_array(struct menuitem item, int n) {
	for (int i = 0; i < n; i++) {
		item.sections[i][0] = '\0';
	}
}

/**
 * Trim whitespace. <http://stackoverflow.com/a/122721/126353>
 *
 * @param str String.
 * @returns Trimmed string.
 */
char *trim(char *str) {
	char *end;

	// Trim leading space
	while(isspace(*str)) str++;

	if(*str == 0)  // All spaces?
		return str;

	// Trim trailing space
	end = str + strlen(str) - 1;
	while(end > str && isspace(*end)) end--;

	// Write new null terminator
	*(end+1) = 0;

	return str;
}

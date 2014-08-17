/**
 *  xdg_menu.h
 *  XDG menu stuff.
 *
 *  @author Nathan Campos <nathanpc@dreamintech.net>
 */

#ifndef _XDG_MENU_H
#define _XDG_MENU_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

struct menuitem {
	char title[128];
	char command[128];
	char section[128];
};

int parse_menus(struct menuitem **items);

#endif

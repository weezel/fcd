#include <menu.h>

#include <err.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "extern.h"
#include "choosewin.h"

int	 returnval = 0;

void
show_chooser_win(MENU *dirmenu, size_t items, char *buf)
{
	int	 c = 0;
	size_t	 buflen, offset = 0;
	char	*tmp;

	buflen = strlen(buf) - 1;

	set_menu_fore(dirmenu, A_REVERSE);
	returnval = set_menu_format(dirmenu, LINES, 1);
	post_menu(dirmenu);
	refresh();

	while ((c = getch()) != 'q') {
		switch(c) {
		case 'j':
		case KEY_DOWN:
			menu_driver(dirmenu, REQ_DOWN_ITEM);
			break;
		case 'k':
		case KEY_UP:
			menu_driver(dirmenu, REQ_UP_ITEM);
			break;
		case KEY_NPAGE:
			menu_driver(dirmenu, REQ_SCR_DPAGE);
			break;
		case KEY_PPAGE:
			menu_driver(dirmenu, REQ_SCR_UPAGE);
			break;
		case KEY_END:
			menu_driver(dirmenu, REQ_LAST_ITEM);
			break;
		case KEY_HOME:
			menu_driver(dirmenu, REQ_FIRST_ITEM);
			break;
		case 10: /* Enter */
			move(20, 0);
			clrtoeol();

			tmp = (char *)item_name(current_item(dirmenu));
			offset = strlcpy(buf, tmp, MAX_CHOICESIZE);
			if (offset >= MAX_CHOICESIZE)
				goto toolong;
			if (strlcpy(buf + offset,
				    item_description(current_item(dirmenu)),
				    MAX_CHOICESIZE - offset))
				goto toolong;

			pos_menu_cursor(dirmenu);
			break;
		}
	}
toolong:
	unpost_menu(dirmenu);
	fprintf(stdout, "shit happens\n");

	fprintf(stdout, "VALUE WAS: %d\n", returnval);
}

void
init_screen(void)
{
	setlocale(LC_ALL, "");

	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
}


void
free_menu_and_items(MENU *m)
{
	size_t	  i, n;
	ITEM	**items;

	items = menu_items(m);
	n = (size_t) item_count(m);

	for (i = 0; i < n; i++)
		free_item(items[i]);
	free_menu(m);
}


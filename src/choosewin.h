#ifndef _CHOOSER_H_
#define _CHOOSER_H_

#if defined __linux__
#include <bsd/string.h>
#endif

#include <menu.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

void	show_chooser_win(MENU *, size_t, char *);
void	init_screen(void);
void	free_menu_and_items(MENU *);
void	free_items(ITEM **, size_t);

#endif

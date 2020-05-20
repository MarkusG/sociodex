#include <stdio.h>

#include <libpq-fe.h>

#include "menu/menus.h"

int main(void) {
	initscr();
	cbreak();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);

	PGconn *conn = PQconnectdb("dbname=mockadex");
	if (!conn) {
		endwin();
		return 1;
	}
	
	MENU *main_menu = init_main_menu(conn);
	menu_opts_off(main_menu, O_SHOWDESC);
	set_menu_mark(main_menu, NULL);

	post_menu(main_menu);

	int c;
	while ((c = getch()) != 'q') {
		switch (c) {
			case KEY_UP:
			case 'k':
				menu_driver(main_menu, REQ_UP_ITEM);
				break;
			case KEY_DOWN:
			case 'j':
				menu_driver(main_menu, REQ_DOWN_ITEM);
				break;
			case KEY_LEFT:
			case 'h':
				menu_driver(main_menu, REQ_LEFT_ITEM);
				break;
			case KEY_RIGHT:
			case 'l':
				menu_driver(main_menu, REQ_RIGHT_ITEM);
				break;
		}
	}

	endwin();
}

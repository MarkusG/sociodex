#include <stdio.h>

#include <libpq-fe.h>

#include "menu/sociodex-menu.h"
#include "view/sociodex-view.h"

int main(void) {
	initscr();
	cbreak();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);

	int term_width, term_height;
	getmaxyx(stdscr, term_height, term_width);

	PGconn *conn = PQconnectdb("dbname=mockadex");
	if (PQstatus(conn) != CONNECTION_OK) {
		endwin();
		printf("could not connect to database\n");
		return 1;
	}
	
	MENU *main_menu = init_main_menu(conn);
	menu_opts_off(main_menu, O_SHOWDESC);
	set_menu_mark(main_menu, NULL);
	set_menu_format(main_menu, term_height, 1);

	post_menu(main_menu);

	int c;
	// TODO keep track of how deep we are in the program to know whether
	// to quit or not; possibly use the ncurses panels library
	while ((c = getch()) != 'q') {
		switch (c) {
			case KEY_UP:
			case 'k': {
				menu_driver(main_menu, REQ_UP_ITEM);
				break;
			}
			case KEY_DOWN:
			case 'j': {
				menu_driver(main_menu, REQ_DOWN_ITEM);
				break;
			}
			case KEY_LEFT:
			case 'h': {
				menu_driver(main_menu, REQ_LEFT_ITEM);
				break;
			}
			case KEY_RIGHT:
			case 'l': {
				menu_driver(main_menu, REQ_RIGHT_ITEM);
				break;
			}
			case '\n':
			case '\r': {
				ITEM *person = current_item(main_menu);
				char *uid = (char*)item_userptr(person);
				unpost_menu(main_menu);
				WINDOW *summary = person_summary_view(conn, stdscr, uid);
				box(summary, 0, 0);
				wrefresh(summary);
				break;
			}
		}
	}

	endwin();
}

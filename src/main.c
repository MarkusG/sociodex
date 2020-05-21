#include <stdio.h>

#include <libpq-fe.h>

#include "menu/sociodex-menu.h"
#include "view/sociodex-view.h"
#include "sociodex-panelstack.h"

int main(void) {
	initscr();
	cbreak();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);

	init_panel_stack();

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
				wrefresh(stdscr);
				WINDOW *summary = person_summary_view(conn, stdscr, uid);
				box(summary, 0, 0);
				if (push_panel(new_panel(summary), PANEL_PERSON_SUMMARY)) {
					endwin();
					fprintf(stderr, "could not push panel onto stack\n");
					return 1;
				}
				update_panels();
				wrefresh(summary);
				break;
			}
			case KEY_BACKSPACE: {
				if (peek_panel().panel) {
					// we're not in the main menu
					PANEL *current = pop_panel().panel;
					PANEL *below = panel_below(current);
					WINDOW *below_win;
					if (!below) {
						// we're above the main menu
						below_win = stdscr;
					} else {
						below_win = panel_window(below);
					}
					werase(panel_window(current));
					delwin(panel_window(current));
					del_panel(current);
					update_panels();
					doupdate();
					if (below_win == stdscr) {
						post_menu(main_menu);
						wrefresh(below_win);
					}
				}
			}

		}
	}

	endwin();
}

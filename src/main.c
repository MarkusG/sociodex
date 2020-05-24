#include <stdio.h>

#include <libpq-fe.h>

#include "menu/sociodex-menu.h"
#include "view/sociodex-view.h"
#include "sociodex-panelstack.h"
#include "sociodex-delegate.h"

int main(void) {
	initscr();
	cbreak();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);

	init_panel_stack();

	PGconn *conn = PQconnectdb("dbname=mockadex");
	if (PQstatus(conn) != CONNECTION_OK) {
		endwin();
		printf("could not connect to database\n");
		return 1;
	}
	
	state state = { .term_width = 0, .term_height = 0, .db_conn = conn };
	getmaxyx(stdscr, state.term_height, state.term_width);

	int c = 0;
	key_delegate delegate = main_menu_delegate;
	void *ptr = NULL;
	void *data = NULL;
	while (true) {
		switch (delegate(c, state, &ptr, &data)) {
			case QUIT:
				endwin();
				return 0;
			case CONTINUE:
				break;
			case CHANGE_DELEGATE:
				delegate = (key_delegate)ptr;
				continue;
			case ERROR:
				endwin();
				fprintf(stderr, "sociodex: %s\n", (char*)ptr);
				return 1;
		}

		c = getch();
	}

	endwin();
}

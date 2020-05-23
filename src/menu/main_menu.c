#include <stdlib.h>
#include <string.h>

#include "../sociodex-delegate.h"
#include "sociodex-menu.h"

MENU *main_menu = NULL;

status delegate(int c, state state, void *ptr) {
	// initialize the main menu if it is not already initialized
	// this should only run once, when the program starts
	if (!main_menu) {
		main_menu = init_main_menu(state.db_conn);
		menu_opts_off(main_menu, O_SHOWDESC);
		set_menu_mark(main_menu, NULL);
		set_menu_format(main_menu, state.term_height, 1);

		post_menu(main_menu);
		return CONTINUE;
	}

	ptr = NULL;
	switch (c) {
		case 'q':
			return QUIT;
		case KEY_UP:
		case 'k':
			menu_driver(main_menu, REQ_UP_ITEM);
			return CONTINUE;
		case KEY_DOWN:
		case 'j':
			menu_driver(main_menu, REQ_DOWN_ITEM);
			return CONTINUE;
		case KEY_LEFT:
		case 'h':
			menu_driver(main_menu, REQ_LEFT_ITEM);
			return CONTINUE;
		case KEY_RIGHT:
		case 'l':
			menu_driver(main_menu, REQ_RIGHT_ITEM);
			return CONTINUE;
		default:
			return CONTINUE;
	}
}

key_delegate main_menu_delegate = &delegate;

MENU *init_main_menu(PGconn *conn) {
	PGresult *query_result = PQexec(conn,
			"SELECT uid, first_name, last_name, gender, phone, met_through "
			"FROM person "
			"ORDER BY last_name;");
	switch (PQresultStatus(query_result)) {
		case PGRES_TUPLES_OK:
			// intended behavior
			break;
		case PGRES_COMMAND_OK:
			// we got no data back
			printw("got no data back");
			return NULL;
		default:
			// unexpected response
			printw(PQresStatus(PQresultStatus(query_result)));
			printw(PQresultErrorMessage(query_result));
			return NULL;
	}

	int n_rows = PQntuples(query_result);
	int n_fields = PQnfields(query_result) - 1; // we don't want to show uid

	int *max_len = (int*)calloc(n_fields, sizeof(int));
	for (int i = 0; i < n_rows; i++) {
		for (int j = 0; j < n_fields; j++) {
			char *field_value = PQgetvalue(query_result, i, j);
			int len = strlen(field_value);
			if (len > max_len[j])
				max_len[j] = len;
		}
	}

	ITEM **items = (ITEM**)calloc(n_rows, sizeof(ITEM*));
	char *row = (char*)malloc(128 * sizeof(char));
	char *padded_value = (char*)malloc(64 * sizeof(char));
	int i;
	for (i = 0; i < n_rows; i++) {
		*row = '\0';
		for (int j = 0; j < n_fields; j++) {
			sprintf(padded_value, "%-*s", max_len[j + 1], PQgetvalue(query_result, i, j + 1));
			strcat(row, padded_value);
			if (j != n_fields - 1)
				strcat(row, " | ");
		}
		items[i] = new_item(strdup(row), NULL);
		set_item_userptr(items[i], PQgetvalue(query_result, i, 0));
	}
	items[i] = (ITEM*)NULL;

	MENU *menu = new_menu(items);
	return menu;
}

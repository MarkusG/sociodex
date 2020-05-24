#include "../sociodex-delegate.h"
#include "sociodex-view.h"

WINDOW *window = NULL;

static status delegate(int c, state state, void **ptr, void **uid) {
	if (!window) {
		window = person_summary_view(state.db_conn, stdscr, (char*)*uid);
		box(window, 0, 0);
		if (push_panel(new_panel(window), PANEL_PERSON_SUMMARY)) {
			*ptr = (void*)"could not push panel onto stack";
			return ERROR;
		}
		update_panels();
		wrefresh(window);
	}

	switch (c) {
		case 'q': {
			PANEL *current = pop_panel().panel;
			werase(panel_window(current));
			delwin(panel_window(current));
			del_panel(current);
			update_panels();
			doupdate();
			window = NULL;
			*ptr = (void*)main_menu_delegate;
			return CHANGE_DELEGATE;
		}
		default:
			return CONTINUE;
	}
}

key_delegate person_summary_delegate = &delegate;

WINDOW *person_summary_view(PGconn *conn,
		WINDOW *parent_window,
		const char *uid) {
	PGresult *person_result = PQexecParams(conn,
			"SELECT first_name, last_name, gender, phone, date_met, met_through "
			"FROM person "
			"WHERE uid = $1;",
			1,    // nParams: parameter count
			NULL, // paramTypes[]: let postgres infer the type
			&uid,
			NULL, // paramLengths[]: not needed for string parameters
			NULL, // paramFormats[]: NULL indicates all strings
			0);   // resultFormat: get results in text format

	switch (PQresultStatus(person_result)) {
		case PGRES_TUPLES_OK:
			// intended behavior
			break;
		case PGRES_COMMAND_OK:
			// we got no data back
			printw("got no data back");
			return NULL;
		default:
			// unexpected response
			printw(PQresStatus(PQresultStatus(person_result)));
			printw("\n");
			printw(PQresultErrorMessage(person_result));
			return NULL;
	}

	PGresult *address_result = PQexecParams(conn,
			"SELECT street_addr, city, state, country, postal_code "
			"FROM address AS a "
			"LEFT JOIN residence AS r ON a.uid = r.address_uid "
			"LEFT JOIN person AS p ON r.person_uid = p.uid "
			"WHERE p.uid = $1 "
			"ORDER BY country, state, postal_code, city;",
			1,    // nParams: parameter count
			NULL, // paramTypes[]: let postgres infer the type
			&uid,
			NULL, // paramLengths[]: not needed for string parameters
			NULL, // paramFormats[]: NULL indicates all strings
			0);   // resultFormat: get results in text format

	switch (PQresultStatus(address_result)) {
		case PGRES_TUPLES_OK:
			// intended behavior
			break;
		case PGRES_COMMAND_OK:
			// we got no data back
			printw("got no data back");
			return NULL;
		default:
			// unexpected response
			printw(PQresStatus(PQresultStatus(address_result)));
			printw("\n");
			printw(PQresultErrorMessage(address_result));
			return NULL;
	}

	int y, x;
	getmaxyx(parent_window, y, x);
	int height = y / 2;
	int width = x / 2;
	int starty = (y - height) / 2;
	int startx = (x - width) / 2;
	WINDOW *window = derwin(parent_window, height, width, starty, startx);
	WINDOW *writespace = derwin(window, height - 2, width - 4, 1, 2);
	werase(window);
	// this could be a loop but a stack of statements is probably better
	// because we have to print a different pattern every time
	int field = 0;
	wprintw(writespace, "Name: %s %s\n",
			PQgetvalue(person_result, 0, field),
			PQgetvalue(person_result, 0, field + 1));
	field += 2;
	wprintw(writespace, "Gender: %s\n",
			PQgetvalue(person_result, 0, field++));
	wprintw(writespace, "Phone: %s\n\n",
			PQgetvalue(person_result, 0, field++));
	wprintw(writespace, "Date met: %s\n",
			PQgetvalue(person_result, 0, field++));
	wprintw(writespace, "Met through: %s\n\n",
			PQgetvalue(person_result, 0, field++));
	wprintw(writespace, "Addresses:\n\n");
	
	for (int i = 0; i < PQntuples(address_result); i++) {
		wprintw(writespace, "%s\n%s, %s, %s %s\n\n",
				PQgetvalue(address_result, i, 0),
				PQgetvalue(address_result, i, 1),
				PQgetvalue(address_result, i, 2),
				PQgetvalue(address_result, i, 3),
				PQgetvalue(address_result, i, 4));
	}

	return window;
}

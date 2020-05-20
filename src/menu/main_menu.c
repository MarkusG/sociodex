#include <stdlib.h>
#include <string.h>

#include "menus.h"

MENU *init_main_menu(PGconn *conn) {
	PGresult *query_result = PQexec(conn,
			"SELECT first_name, last_name, gender, phone, met_through "
			"FROM person;");
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
	int n_fields = PQnfields(query_result);

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
			sprintf(padded_value, "%-*s", max_len[j], PQgetvalue(query_result, i, j));
			strcat(row, padded_value);
			if (j != n_fields - 1)
				strcat(row, " | ");
		}
		items[i] = new_item(strdup(row), NULL);
	}
	items[i] = (ITEM*)NULL;

	MENU *menu = new_menu(items);
	return menu;
}

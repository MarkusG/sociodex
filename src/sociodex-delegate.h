#ifndef __SOCIODEX_DELEGATE_H__
#define __SOCIODEX_DELEGATE_H__

#include <libpq-fe.h>

typedef enum status {
	ERROR,
	QUIT,
	CONTINUE,
	CHANGE_DELEGATE
} status;

typedef struct state {
	int term_width;
	int term_height;
	
	PGconn *db_conn;
} state;

typedef status (*key_delegate)(int, state, void*);
#endif

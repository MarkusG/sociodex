#ifndef __SOCIODEX_VIEW_H__
#define __SOCIODEX_VIEW_H__
#include <curses.h>
#include <libpq-fe.h>

WINDOW *person_summary_view(PGconn*, WINDOW*, const char*);
#endif

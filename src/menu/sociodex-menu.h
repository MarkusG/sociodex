#ifndef __SOCIODEX_MENU_H__
#define __SOCIODEX_MENU_H__
#include <menu.h>
#include <libpq-fe.h>

MENU *init_main_menu(PGconn *conn);
int free_menu_items(const MENU*);
#endif

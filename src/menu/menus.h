#ifndef __MENUS_H__
#define __MENUS_H__
#include <menu.h>
#include <libpq-fe.h>

MENU *init_main_menu(PGconn *conn);
#endif

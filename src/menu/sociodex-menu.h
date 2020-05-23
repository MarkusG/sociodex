#ifndef __SOCIODEX_MENU_H__
#define __SOCIODEX_MENU_H__
#include <menu.h>
#include <libpq-fe.h>

#include "../sociodex-delegate.h"

extern key_delegate main_menu_delegate;

MENU *init_main_menu(PGconn *conn);
#endif

#ifndef __SOCIODEX_VIEW_H__
#define __SOCIODEX_VIEW_H__
#include <panel.h>
#include <libpq-fe.h>

#include "../sociodex-panelstack.h"

WINDOW *person_summary_view(PGconn*, WINDOW*, const char*);
#endif

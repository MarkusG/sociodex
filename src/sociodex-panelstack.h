#ifndef __SOCIODEX_PANELSTACK_H__
#define __SOCIODEX_PANELSTACK_H__
#include <panel.h>

int init_panel_stack(void);
int push_panel(PANEL*);
PANEL* pop_panel(void);
PANEL* peek_panel(void);
#endif

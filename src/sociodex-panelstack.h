#ifndef __SOCIODEX_PANELSTACK_H__
#define __SOCIODEX_PANELSTACK_H__
#include <panel.h>

typedef enum panel_type {
	PANEL_BLANK,
	PANEL_MAIN_MENU,
	PANEL_PERSON_SUMMARY
} panel_type;

typedef struct sociodex_panel {
	PANEL *panel;
	panel_type type;
} sociodex_panel;

int init_panel_stack(void);
int push_panel(PANEL*, panel_type);
sociodex_panel pop_panel(void);
sociodex_panel peek_panel(void);
#endif

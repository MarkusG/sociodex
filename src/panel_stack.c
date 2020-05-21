#include <stdlib.h>

#include "sociodex-panelstack.h"

#define PANEL_STACK_BUFSIZE 4

const sociodex_panel BLANK_PANEL = { .panel = NULL, .type = PANEL_BLANK };

static sociodex_panel *stack;
static int capacity = 0, n_panels = 0;

int resize_stack(void) {
	capacity += PANEL_STACK_BUFSIZE;
	stack = (sociodex_panel*)realloc(stack, capacity * sizeof(sociodex_panel));
	if (!stack) {
		perror("sociodex");
		return 1;
	}
	return 0;
}

int init_panel_stack(void) {
	capacity = PANEL_STACK_BUFSIZE;
	stack = (sociodex_panel*)malloc(capacity * sizeof(sociodex_panel));
	if (!stack) {
		perror("sociodex");
		return 1;
	}
	n_panels = 0;
	return 0;
}

int push_panel(PANEL *panel, panel_type type) {
	if (capacity == 0) {
		return 1;
	}
	if (n_panels == capacity) {
		if (resize_stack()) {
			perror("sociodex");
			return 1;
		}
	}
	sociodex_panel s_panel = { .panel = panel, .type = type };
	stack[n_panels++] = s_panel;
	return 0;
}

sociodex_panel pop_panel(void) {
	if (n_panels == 0)
		return BLANK_PANEL;
	return stack[--n_panels];
}

sociodex_panel peek_panel(void) {
	if (n_panels == 0)
		return BLANK_PANEL;
	return stack[n_panels - 1];
}

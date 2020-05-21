#include <stdlib.h>

#include "sociodex-panelstack.h"

#define PANEL_STACK_BUFSIZE 4

static PANEL **stack;
static int capacity, n_panels;

int resize_stack(void) {
	capacity += PANEL_STACK_BUFSIZE;
	stack = (PANEL**)realloc(stack, capacity * sizeof(PANEL*));
	if (!stack) {
		perror("sociodex");
		return 1;
	}
	return 0;
}

int init_panel_stack(void) {
	capacity = PANEL_STACK_BUFSIZE;
	stack = (PANEL**)malloc(capacity * sizeof(PANEL*));
	if (!stack) {
		perror("sociodex");
		return 1;
	}
	n_panels = 0;
	return 0;
}

int push_panel(PANEL *panel) {
	if (n_panels == capacity) {
		if (resize_stack()) {
			perror("sociodex");
			return 1;
		}
	}
	stack[n_panels++] = panel;
	return 0;
}

PANEL *pop_panel(void) {
	if (n_panels == 0)
		return NULL;
	return stack[--n_panels];
}

PANEL *peek_panel(void) {
	if (n_panels == 0)
		return NULL;
	return stack[n_panels - 1];
}

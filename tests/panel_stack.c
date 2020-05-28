#include <stdio.h>

#include "../src/panel-stack.h"

int main(void) {
	if (init_panel_stack())
		return 1;
	for (long int i = 0; i < 128; i++) {
		// we don't care about actual panel functionality, just our stack logic
		PANEL *panel = (PANEL*)i;
		push_panel(panel, PANEL_BLANK);
	}
}

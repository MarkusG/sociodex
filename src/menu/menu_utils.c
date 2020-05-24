#include <stdlib.h>

#include "sociodex-menu.h"

int free_menu_items(const MENU* menu) {
	int result = 0;
	ITEM **items = menu_items(menu);
	for (int i = 0; i < item_count(menu); i++) {
		if (free_item(items[i]) != E_OK)
			result = 1;
	}
	return result;
}

#include <stdlib.h>

#include "menu-utils.h"

int free_menu_items(const MENU* menu) {
	int result = 0;
	ITEM **items = menu_items(menu);
	for (int i = 0; i < item_count(menu); i++) {
		free(item_name(items[i]));
		free(item_description(items[i]));
		free(item_userptr(items[i]));
		if (free_item(items[i]) != E_OK)
			result = 1;
		free(items[i]);
	}
	free(items);
	return result;
}

#ifndef MENU_H
#define MENU_H

#include "obj.h"

struct CURSOR {
  struct OBJ *obj;
};

extern struct CURSOR cursor;

void menu_update(void);

void menu_init(void);

#endif /* MENU_H */

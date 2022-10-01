//
// Created by vic485 on 22/10/01.
//

#ifndef TOUHOUGAME_ARRAYLIST_H
#define TOUHOUGAME_ARRAYLIST_H

#include "../types.h"

typedef struct array_list {
    usize length;
    usize capacity;
    usize itemSize;
    void *items;
} ArrayList;

ArrayList *arrayListCreate(usize itemSize, usize initialCapacity);

usize arrayListAppend(ArrayList *list, void *item);

void *arrayListGet(ArrayList *list, usize index);

u8 arrayListRemove(ArrayList *list, usize index);

void arrayListFree(ArrayList *list);

#endif //TOUHOUGAME_ARRAYLIST_H

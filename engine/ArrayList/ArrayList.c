//
// Created by vic485 on 22/10/01.
//

#include "ArrayList.h"
#include "../util.h"

#include <stdlib.h>
#include <string.h>

ArrayList *arrayListCreate(usize itemSize, usize initialCapacity) {
    ArrayList *list = malloc(sizeof(ArrayList));

    if (!list) ERROR_RETURN(NULL, "Could not allocate memory for ArrayList")

    list->itemSize = itemSize;
    list->capacity = initialCapacity;
    list->length = 0;
    list->items = malloc(itemSize * initialCapacity);

    if (!list->items) ERROR_RETURN(NULL, "Could not allocate memory for ArrayList")

    return list;
}

usize arrayListAppend(ArrayList *list, void *item) {
    if (list->length == list->capacity) {
        list->capacity = list->capacity > 0 ? list->capacity * 2 : 1;
        void *items = realloc(list->items, list->itemSize * list->capacity);

        if (!items) ERROR_RETURN(-1, "Could not allocate memory for ArrayList\n")

        list->items = items;
    }

    usize index = list->length++;

    memcpy((u8 *) list->items + index * list->itemSize, item, list->itemSize);

    return index;
}

void *arrayListGet(ArrayList *list, usize index) {
    if (index >= list->length) ERROR_RETURN(NULL, "Index out of bounds")

    return (u8 *) list->items + index * list->itemSize;
}

u8 arrayListRemove(ArrayList *list, usize index) {
    if (list->length == 0) ERROR_RETURN(1, "List is empty\n")
    if (index >= list->length) ERROR_RETURN(1, "Index out of bounds\n")

    if (list->length == 1) {
        list->length = 0;
        return 0;
    }

    --list->length;

    u8 *item_ptr = (u8 *) list->items + index * list->itemSize;
    u8 *end_ptr = (u8 *) list->items + list->length * list->itemSize;
    memcpy(item_ptr, end_ptr, list->itemSize);

    return 0;
}

void arrayListFree(ArrayList *list) {
    free(list->items);
    free(list);
}

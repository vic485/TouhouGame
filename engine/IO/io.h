//
// Created by vic485 on 22/10/01.
//

#ifndef TOUHOUGAME_IO_H
#define TOUHOUGAME_IO_H

#include <stdlib.h>
#include <stdbool.h>
#include "../types.h"

typedef struct file {
    char *data;
    usize length;
    bool isValid;
} File;

File io_fileRead(const char *path);

int io_fileWrite(void *buffer, usize size, const char *path);

#endif //TOUHOUGAME_IO_H

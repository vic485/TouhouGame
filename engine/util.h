//
// Created by vic485 on 22/10/01.
//

#ifndef TOUHOUGAME_UTIL_H
#define TOUHOUGAME_UTIL_H

#include <log.h>

#define ERROR_EXIT(...) { log_error(__VA_ARGS__); exit(1); }
#define ERROR_RETURN(R, ...) { log_error(__VA_ARGS__); return R; }

#endif //TOUHOUGAME_UTIL_H

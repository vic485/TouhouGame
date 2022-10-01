## External Dependencies
# Glad
add_library(glad glad/src/glad.c)
target_include_directories(glad PUBLIC glad/include)

# linmath
add_library(linmath linmath/linmath.c)
target_include_directories(linmath INTERFACE linmath)

# log.c
add_library(logc log.c/src/log.c)
target_include_directories(logc INTERFACE log.c/src)

# QOI
add_library(qoi qoi/qoi.c)
target_include_directories(qoi INTERFACE qoi)

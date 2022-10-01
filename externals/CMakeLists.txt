## External Dependencies
# Glad
add_library(glad glad/src/glad.c)
target_include_directories(glad PUBLIC glad/include)

# log.c
add_library(logc log.c/src/log.c)
target_include_directories(logc INTERFACE log.c/src)

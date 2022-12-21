add_library(glad src/glad.c)

project(glad)

target_include_directories(${PROJECT_NAME} PUBLIC ../include)


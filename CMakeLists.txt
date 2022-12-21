cmake_minimum_required(VERSION 3.23.1)

project(StarDwarves)

file(GLOB_RECURSE SOURCE_FILES 
	${CMAKE_SOURCE_DIR}/src/*.c
	${CMAKE_SOURCE_DIR}/src/*.cpp)

# add header files
file(GLOB_RECURSE HEADER_FILES 
	${CMAKE_SOURCE_DIR}/include/*.h
	${CMAKE_SOURCE_DIR}/include/*.hpp
	${CMAKE_SOURCE_DIR}/include/*.cpp)
include_directories(${INCLUDE_DIR})
include_directories(
	"${CMAKE_SOURCE_DIR}/src"
	"${CMAKE_SOURCE_DIR}/include"
)
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})





add_subdirectory(lib/glm)
add_subdirectory(lib/glad)
add_subdirectory(lib/glfw)
add_subdirectory(lib/assimp)
target_link_directories(${PROJECT_NAME} PRIVATE include/)


target_link_libraries(${PROJECT_NAME} glm glad glfw assimp)

include_directories(${PROJECT_NAME} .lib/)



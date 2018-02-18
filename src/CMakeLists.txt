cmake_minimum_required(VERSION 3.8)

project(wwwfs)

add_definitions(-DFUSE_USE_VERSION=31)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)

file(GLOB_RECURSE HEADERS *.h)
file(GLOB_RECURSE SOURCES *.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Network)

find_package(PkgConfig REQUIRED)
pkg_search_module(FUSE3 REQUIRED fuse3)

target_link_libraries(${PROJECT_NAME}  ${FUSE3_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC ${FUSE3_INCLUDE_DIRS})
target_compile_options(${PROJECT_NAME} PUBLIC ${FUSE3_CFLAGS_OTHER})

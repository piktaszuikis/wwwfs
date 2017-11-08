cmake_minimum_required(VERSION 3.8)

project(httpfs)

set(SOURCES
	main.c
	wwwfs-fuse.c
	wwwfs-curl.c
	wwwfs-parser.c)

set(HEADERS
    wwwfs-fuse.h
	wwwfs-curl.h
	wwwfs-parser.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

find_package(PkgConfig REQUIRED)
pkg_search_module(FUSE3 REQUIRED fuse3)

target_link_libraries(${PROJECT_NAME}  ${FUSE3_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC ${FUSE3_INCLUDE_DIRS})
target_compile_options(${PROJECT_NAME} PUBLIC ${FUSE3_CFLAGS_OTHER})

pkg_search_module(CURL REQUIRED libcurl)
target_link_libraries(${PROJECT_NAME}  ${CURL_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CURL_INCLUDE_DIRS})
target_compile_options(${PROJECT_NAME} PUBLIC ${CURL_CFLAGS_OTHER})

find_package(LibXml2)
target_link_libraries(${PROJECT_NAME}  ${LIBXML2_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC ${LIBXML2_INCLUDE_DIR})
target_compile_options(${PROJECT_NAME} PUBLIC ${LIBXML2_DEFINITIONS})


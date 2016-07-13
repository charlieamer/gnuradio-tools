cmake_minimum_required(VERSION 2.6)
project(gnuradio_udp_server)

find_package (Boost 1.55.0 REQUIRED COMPONENTS thread)
include_directories (SYSTEM ${Boost_INCLUDE_DIR})
add_library(gnuradio_udp_server STATIC src/server.cpp)

add_subdirectory(include)
add_subdirectory(src)

set(config_install_dir "lib/cmake/${PROJECT_NAME}")
set(include_install_dir "include")

get_filename_component(PROJECT_PATH "${CMAKE_CURRENT_LIST_FILE}" PATH)
set(BUILD_PATH ${CMAKE_CURRENT_BINARY_DIR})
configure_file(gnuradio_udp_server-config.cmake.in gnuradio_udp_server-config.cmake)

target_include_directories(
    gnuradio_udp_server PUBLIC
    include)

install(
    FILES "${BUILD_PATH}/gnuradio_udp_server-config.cmake"
    DESTINATION "${config_install_dir}"
)

install(
    FILES "include/server.h"
    DESTINATION "include"
)

install(TARGETS gnuradio_udp_server
ARCHIVE DESTINATION lib
INCLUDES DESTINATION include)
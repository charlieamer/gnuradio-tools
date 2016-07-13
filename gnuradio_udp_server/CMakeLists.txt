cmake_minimum_required(VERSION 2.6)
project(gnuradio_udp_server)

find_package (Boost 1.55.0 REQUIRED COMPONENTS thread)
include_directories (SYSTEM ${Boost_INCLUDE_DIR})
add_library(gnuradio_udp_server STATIC src/server.cpp)

add_subdirectory(include)
add_subdirectory(src)
get_filename_component(PROJECT_PATH "${CMAKE_CURRENT_LIST_FILE}" PATH)
set(BUILD_PATH ${CMAKE_CURRENT_BINARY_DIR})
configure_file(gnuradio_udp_server-config.cmake.in gnuradio_udp_server-config.cmake)
install(TARGETS gnuradio_udp_server ARCHIVE DESTINATION lib)
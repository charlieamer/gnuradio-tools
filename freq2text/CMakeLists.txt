cmake_minimum_required(VERSION 2.6)
project(freq2text)

find_package (Boost 1.55.0 REQUIRED COMPONENTS thread)
find_package (gnuradio_udp_server REQUIRED)
find_package (Threads)

include_directories (SYSTEM ${Boost_INCLUDE_DIR})
include_directories (${gnuradio_udp_server_INCLUDE_DIR})

add_executable(${PROJECT_NAME} main.cpp biquad.cpp)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
target_link_libraries (${PROJECT_NAME} ${Boost_LIBRARIES} ${gnuradio_udp_server_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} rt)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_range_for cxx_generalized_initializers)
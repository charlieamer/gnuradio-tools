cmake_minimum_required(VERSION 2.6)
project(decode)

find_package (Boost 1.55.0 REQUIRED COMPONENTS program_options)
include_directories (SYSTEM ${Boost_INCLUDE_DIR})

add_executable(decode main.cpp decoder.cpp binary.cpp manchester.cpp header.cpp)

install(TARGETS decode RUNTIME DESTINATION bin)
TARGET_LINK_LIBRARIES( decode ${Boost_LIBRARIES} )
target_compile_features(decode PUBLIC cxx_override)
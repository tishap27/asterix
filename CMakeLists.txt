cmake_minimum_required(VERSION 3.16)
project(asterix_decode CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(asterix_lib src/asterix.cpp)
target_include_directories(asterix_lib PUBLIC src)
target_compile_options(asterix_lib PRIVATE -Wall -Wextra)

add_executable(asterix src/main.cpp)
target_link_libraries(asterix PRIVATE asterix_lib)
target_compile_options(asterix PRIVATE -Wall -Wextra)

enable_testing()
add_executable(test_run src/test.cpp)
target_link_libraries(test_run PRIVATE asterix_lib)
# the tests use assert(), so keep asserts on even in Release builds
target_compile_options(test_run PRIVATE -Wall -Wextra -UNDEBUG)

add_test(NAME unit_tests COMMAND test_run)
add_test(NAME sample_capture COMMAND asterix -f ${CMAKE_SOURCE_DIR}/samples/capture.txt)
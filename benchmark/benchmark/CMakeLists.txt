cmake_minimum_required(VERSION 3.8)# or 3.25
project(benchmarkFormula LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCES     
   benchmarkFormula.cpp
)
    
set(HEADER)

# Скачиваем CPM.cmake (можно вручную скачать и положить в соответсвующую папку)
# file(
#     DOWNLOAD
#     https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.38.3/CPM.cmake
#     ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
#     EXPECTED_HASH SHA256=cc155ce02e7945e7b8967ddfaff0b050e958a723ef7aad3766d368940cb15494
# )
# include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)

# Скачиваем библиотеку cpptrace
# CPMAddPackage("gh:jeremy-rifkin/cpptrace#v0.7.2")

message(
"\n=============================================================
!!!!          CMAKE_BUILD_TYPE = "${CMAKE_BUILD_TYPE}"                 !!!!
=============================================================\n")
add_subdirectory(CppBenchmark)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADER})
target_link_libraries(${PROJECT_NAME} PUBLIC cppbenchmark)

# Install and link cppbenchmark:
# git clone https://github.com/chronoxor/CppBenchmark.git
# cd CppBenchmark
# gil update

# If gil not found, then:
# sudo pip3 install --prefix /usr/local gil
# echo 'PATH="$HOME/.local/bin/:$PATH"' >> ~/.bashrc
# pip3 install gil

# cd build
# ./unix.sh

# Create a new *.cpp file
# Insert #include "benchmark/cppbenchmark.h"

# Copy-paste example benchmark source code in cppbenchmark
# Create CMakeLists.txt and add target link "cppbenchmark"

# Compile
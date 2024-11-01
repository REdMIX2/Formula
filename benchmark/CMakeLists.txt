cmake_minimum_required(VERSION 3.20)
project(benchmarksFormula LANGUAGES CXX)

# Global properties
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Add benchmark lib
add_subdirectory(CppBenchmark)

# Compile examples
file(GLOB EXAMPLE_HEADER_FILES "examples/*.h")
file(GLOB EXAMPLE_INLINE_FILES "examples/*.inl")
file(GLOB EXAMPLE_SOURCE_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/examples" "examples/*.cpp")

set(INSTALL_TARGETS "")
set(INSTALL_TARGETS_PDB "")

foreach(EXAMPLE_SOURCE_FILE ${EXAMPLE_SOURCE_FILES})
   string(REGEX REPLACE "(.*)\\.cpp" "\\1" EXAMPLE_NAME ${EXAMPLE_SOURCE_FILE})
   set(EXAMPLE_TARGET "formula-example-${EXAMPLE_NAME}")

   add_executable(${EXAMPLE_TARGET} ${EXAMPLE_HEADER_FILES} ${EXAMPLE_INLINE_FILES} "examples/${EXAMPLE_SOURCE_FILE}")
   set_target_properties(${EXAMPLE_TARGET} PROPERTIES COMPILE_FLAGS "${PEDANTIC_COMPILE_FLAGS}" FOLDER "benchmarksFormulaExamples")
   
   target_link_libraries(${EXAMPLE_TARGET} PUBLIC cppbenchmark)

   list(APPEND INSTALL_TARGETS ${EXAMPLE_TARGET})
   list(APPEND INSTALL_TARGETS_PDB ${EXAMPLE_TARGET})
endforeach()

# Install
install(TARGETS ${INSTALL_TARGETS}
   RUNTIME DESTINATION bin
   LIBRARY DESTINATION bin
   ARCHIVE DESTINATION bin)

# Install *.pdb files
if(MSVC)
   foreach(INSTALL_TARGET_PDB ${INSTALL_TARGETS_PDB})
      install(FILES $<TARGET_PDB_FILE:${INSTALL_TARGET_PDB}> DESTINATION "${PROJECT_SOURCE_DIR}/bin")
   endforeach()
endif()


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
include(CheckCXXCompilerFlag)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(OPTIONS /W4 /WX)
    check_cxx_compiler_flag(/permissive HAS_PERMISSIVE_FLAG)
    if(HAS_PERMISSIVE_FLAG)
        set(OPTIONS ${OPTIONS} /permissive-)
    endif()
    check_cxx_compiler_flag(/std:c++20 HAS_CPP20_FLAG)
    check_cxx_compiler_flag(/std:c++23 HAS_CPP23_FLAG)
    check_cxx_compiler_flag(/std:c++latest HAS_CPPLATEST_FLAG)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_VERBOSE_MAKEFILE ON)
    set(OPTIONS -Wall -Wextra -pedantic-errors -Werror)

    check_cxx_compiler_flag(-std=c++20 HAS_CPP20_FLAG)
    check_cxx_compiler_flag(-std=c++23 HAS_CPP23_FLAG)
endif()

CPMAddPackage("gh:catchorg/Catch2@3.4.0")

function(make_test src target std)
    add_executable(${target} ${src})
    target_compile_options(${target} PRIVATE ${OPTIONS})
    # target_include_directories(${target} PRIVATE ${CATCH_SINGLE_INCLUDE})
    target_link_libraries(${target}  PUBLIC Catch2::Catch2WithMain)
    target_link_libraries(${target} PRIVATE ${CMAKE_PROJECT_NAME})
    set_target_properties(${target} PROPERTIES CXX_EXTENSIONS OFF)
    if(std)
        if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
            target_compile_options(${target} PRIVATE /std:${std})
        else()
            target_compile_options(${target} PRIVATE -std=${std})
        endif()
    endif()
    enable_testing()
    add_test(NAME ${target} COMMAND ${target})
endfunction()


function(make_tests_recurse cppVersion)
    file(GLOB_RECURSE SOURCE_TESTS "*.cpp")
    foreach(SOURCE_PATH_FILE ${SOURCE_TESTS})
        get_filename_component(SOURCE_FILE_NAME ${SOURCE_PATH_FILE} NAME)
        string(REPLACE "_" "-" SOURCE_FILE_NAME ${SOURCE_FILE_NAME}) # replace "_" --> "-"
        string(REPLACE ".cpp" "" SOURCE_FILE_NAME ${SOURCE_FILE_NAME}) # replace ".cpp" --> ""
        make_test(${SOURCE_PATH_FILE} ${SOURCE_FILE_NAME}-${cppVersion} ${cppVersion})
    endforeach()
endfunction()


make_tests_recurse("c++${CMAKE_CXX_STANDARD}")



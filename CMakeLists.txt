# cmake_minimum_required(VERSION 3.8)
# set(CMAKE_CXX_STANDARD 17)

# include(GNUInstallDirs)

# set(ADDITIONAL_MODULES_DIR "${CMAKE_CURRENT_LIST_DIR}/cmake")
# list(APPEND CMAKE_MODULE_PATH "${ADDITIONAL_MODULES_DIR}")

# project(formula
#     VERSION "1.0.3"
#     HOMEPAGE_URL "https://github.com/REdMIX2/Formula"
#     DESCRIPTION "A library that provides macros and functions to obtain the name of a variable, type, function, macro, and enum."
#     LANGUAGES CXX
# )

# set(CPACK_PACKAGE_VENDOR "Time-Coder and Egor Petnichuk")

# if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
#     set(IS_TOPLEVEL_PROJECT TRUE)
# else()
#     set(IS_TOPLEVEL_PROJECT FALSE)
# endif()

# option(FORMULA_OPT_BUILD_EXAMPLES "Build formula examples" ${IS_TOPLEVEL_PROJECT})
# option(FORMULA_OPT_BUILD_TESTS "Build and perform formula tests" ${IS_TOPLEVEL_PROJECT})
# option(FORMULA_OPT_INSTALL "Generate and install formula target" ${IS_TOPLEVEL_PROJECT})

# # Устанавливаем пути для исходных файлов и заголовков
# set(INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include")
# set(SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src")

# # Список исходных файлов для компиляции
# file(GLOB_RECURSE SOURCES "${SOURCE}/*.cpp" "${SOURCE}/*.c")

# # Создаем статическую библиотеку с именем проекта
# add_library("${PROJECT_NAME}" STATIC ${SOURCES})

# # Определяем пространство имен для экспорта
# set(EXPORT_NAMESPACE "${PROJECT_NAME}::")

# # Устанавливаем пути для заголовочных файлов
# target_include_directories(${PROJECT_NAME}
#     PUBLIC
#         $<BUILD_INTERFACE:${INCLUDES}>
#         $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
# )

# if(FORMULA_OPT_BUILD_EXAMPLES)
#     add_subdirectory(Example)
# endif()

# if(FORMULA_OPT_INSTALL)
#     include(CMakePackageConfigHelpers)

#     # Установка библиотеки и заголовков в локальные каталоги
#     install(TARGETS "${PROJECT_NAME}"
#         EXPORT ${PROJECT_NAME}Targets
#         INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
#         ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
#         LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
#         RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
#     )

#     # Установка заголовочных файлов
#     file(GLOB_RECURSE HEADERS "${INCLUDES}/*.h" "${INCLUDES}/*.hpp" "${INCLUDES}/*.hxx")
#     foreach(headerFile ${HEADERS})
#         get_filename_component(headerFileParentDir "${headerFile}" DIRECTORY)
#         file(RELATIVE_PATH headerFileRelParentDir "${INCLUDES}" "${headerFileParentDir}")

#         install(FILES "${headerFile}"
#             DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${headerFileRelParentDir}"
#         )
#     endforeach()

#     # Генерация и установка конфигурационного файла пакета
#     install(EXPORT ${PROJECT_NAME}Targets
#         FILE "${PROJECT_NAME}Config.cmake"
#         NAMESPACE "${EXPORT_NAMESPACE}"
#         DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
#     )

#     write_basic_package_version_file(
#         "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
#         VERSION ${PROJECT_VERSION}
#         COMPATIBILITY AnyNewerVersion
#     )

#     install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
#         DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
#     )

#     include(CPack)
# endif()

cmake_minimum_required(VERSION 3.8)

project(formula LANGUAGES CXX)

add_library(formula STATIC
    src/formula.cpp
    src/built_in.hpp
    src/formula_exeption.cpp
)

target_include_directories(formula PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_target_properties(formula PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)
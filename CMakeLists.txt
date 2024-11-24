cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)
# include(GNUInstallDirs)

set(ADDITIONAL_MODULES_DIR "${CMAKE_CURRENT_LIST_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${ADDITIONAL_MODULES_DIR}")

project(formula
    VERSION "1.0.1"
    HOMEPAGE_URL "https://github.com/REdMIX2/Formula"
    DESCRIPTION "A library that provides macros and functions to obtain the name of a variable, type, function, macro, and enum."
    LANGUAGES CXX
)

# Set global variables 
set_property(GLOBAL PROPERTY MAIN_PROJECT_NAME ${PROJECT_NAME})

set(CPACK_PACKAGE_VENDOR "Time-Coder and Egor Petnichuk")

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(IS_TOPLEVEL_PROJECT TRUE)
else()
    set(IS_TOPLEVEL_PROJECT FALSE)
endif()

option(FORMULA_OPT_BUILD_EXAMPLES "Build formula examples" ${IS_TOPLEVEL_PROJECT})
option(FORMULA_OPT_BUILD_TESTS "Build and perform formula tests" ${IS_TOPLEVEL_PROJECT})
option(FORMULA_OPT_INSTALL "Generate and install formula target" ${IS_TOPLEVEL_PROJECT})

# Устанавливаем пути для исходных файлов и заголовков
set(INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Список исходных файлов для компиляции
file(GLOB_RECURSE SOURCES "${SOURCE}/*.cpp" "${SOURCE}/*.c")

# Создаем статическую библиотеку с именем проекта
add_library("${PROJECT_NAME}" STATIC ${SOURCES})

# Определяем пространство имен для экспорта
set(EXPORT_NAMESPACE "${PROJECT_NAME}::")

# Устанавливаем пути для заголовочных файлов
target_include_directories(${PROJECT_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${INCLUDES}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

if(FORMULA_OPT_BUILD_EXAMPLES OR FORMULA_OPT_BUILD_TESTS)
    # Скачиваем CPM.cmake (можно вручную скачать и положить в соответсвующую папку)
    file(
        DOWNLOAD
        https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.38.3/CPM.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
        EXPECTED_HASH SHA256=cc155ce02e7945e7b8967ddfaff0b050e958a723ef7aad3766d368940cb15494
    )
    include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)
endif()

if(FORMULA_OPT_BUILD_EXAMPLES)
    add_subdirectory(Example)
    add_subdirectory(benchmark)
endif()

# if(FORMULA_OPT_BUILD_TESTS)
#     enable_testing()
#     add_subdirectory(tests)
# endif()

if(FORMULA_OPT_INSTALL)
    include(CMakePackageConfigHelpers)

    # Установка библиотеки и заголовков в локальные каталоги
    install(TARGETS "${PROJECT_NAME}"
        EXPORT ${PROJECT_NAME}Targets
        INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    )

    # Установка заголовочных файлов
    file(GLOB_RECURSE HEADERS "${INCLUDES}/*.h" "${INCLUDES}/*.hpp" "${INCLUDES}/*.hxx")
    foreach(headerFile ${HEADERS})
        get_filename_component(headerFileParentDir "${headerFile}" DIRECTORY)
        file(RELATIVE_PATH headerFileRelParentDir "${INCLUDES}" "${headerFileParentDir}")

        install(FILES "${headerFile}"
            DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${headerFileRelParentDir}"
        )
    endforeach()

    # Генерация и установка конфигурационного файла пакета
    install(EXPORT ${PROJECT_NAME}Targets
        FILE "${PROJECT_NAME}Config.cmake"
        NAMESPACE "${EXPORT_NAMESPACE}"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
    )

    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
    )

    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
    )

    include(CPack)
endif()










# # Установка стандартов языка
# set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # Добавляем заголовочный файл для библиотеки
# include_directories()

# # Добавляем библиотеку формулы и указываем все исходные файлы
# set(SOURCE
#     src/formula.cpp
#     src/built_in.cpp
# )

# add_library(formula STATIC ${SOURCE})

# # Опционально: установка путей и экспорт библиотеки
# install(TARGETS formula DESTINATION lib)
# install(DIRECTORY include/ DESTINATION)

# # Создаем настройки для использования библиотеки
# include(CMakePackageConfigHelpers)
# write_basic_package_version_file(
#     "${CMAKE_CURRENT_BINARY_DIR}/formulaConfigVersion.cmake"
#     VERSION ${PROJECT_VERSION}
#     COMPATIBILITY AnyNewerVersion
# )
# export(PACKAGE formula)

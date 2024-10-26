cmake_minimum_required(VERSION 3.27)

include(GNUInstallDirs)

set(ADDITIONAL_MODULES_DIR "${CMAKE_CURRENT_LIST_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${ADDITIONAL_MODULES_DIR}")

project(formula
    VERSION "1.0.1"
    HOMEPAGE_URL "https://github.com/REdMIX2/Formula"
    DESCRIPTION "A library that provides macros and functions to obtain the name of a variable, type, function, macro, and enum."
    LANGUAGES CXX
)

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

if(FORMULA_OPT_BUILD_EXAMPLES)
    add_subdirectory(Example)
endif()

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



# cmake_minimum_required(VERSION 3.27)

# include(GNUInstallDirs)

# set(ADDITIONAL_MODULES_DIR "${CMAKE_CURRENT_LIST_DIR}/cmake")
# list(APPEND CMAKE_MODULE_PATH "${ADDITIONAL_MODULES_DIR}")

# project(formula
#     VERSION "1.0.1"
#     HOMEPAGE_URL "https://github.com/REdMIX2/Formula"
#     DESCRIPTION "A library that provides nameof macros and functions to simply obtain the name of a variable, type, function, macro, and enum."
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

# include(CMakePackageConfigHelpers)

# set(EXPORT_NAMESPACE "${PROJECT_NAME}::")

# file(GLOB_RECURSE SOURCES "${SOURCE}/*.c" "${SOURCE}/*.cpp")
# add_library("${PROJECT_NAME}" INTERFACE)
# add_library("${EXPORT_NAMESPACE}${PROJECT_NAME}" ALIAS "${PROJECT_NAME}")
# set(INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include")
# set(SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src")

# target_sources(${PROJECT_NAME}
#     PRIVATE
#         ${SOURCES}
# )
# # foreach(sourceFile ${SOURCES})
# #     get_filename_component(sourceFileParentDir "${sourceFile}" DIRECTORY)
# #     file(RELATIVE_PATH sourceFileRelParentDir "${SOURCE}" "${sourceFileParentDir}")

# #     install(FILES "${sourceFile}"
# #         DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${sourceFileRelParentDir}"
# #     )
# # endforeach()

# if(FORMULA_OPT_BUILD_EXAMPLES)
#     add_subdirectory(Example)
# endif()

# # if(NAMEOF_OPT_BUILD_TESTS)
# #     enable_testing()
# #     add_subdirectory(test)
# # endif()

# target_include_directories(${PROJECT_NAME}
#     INTERFACE
#     $<BUILD_INTERFACE:${INCLUDES}>
#     $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
# )
# # target_include_directories(${PROJECT_NAME}
# #     PRIVATE
# #         # where the library itself will look for its internal headers
# #         ${SOURCE}
# #     PUBLIC
# #         # where top-level project will look for the library's public headers
# #         $<BUILD_INTERFACE:${INCLUDES}>
# #         # where external projects will look for the library's public headers
# #         $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
# # )

# if(FORMULA_OPT_INSTALL)
#     list(APPEND CMAKE_MODULE_PATH "${ADDITIONAL_MODULES_DIR}/GenPkgConfig")
#     include(GenPkgConfig)
#     include(CPackComponent)
#     include(CMakePackageConfigHelpers)

#     string(REPLACE "/${CMAKE_LIBRARY_ARCHITECTURE}" "" CMAKE_INSTALL_LIBDIR_ARCHIND "${CMAKE_INSTALL_LIBDIR}")
#     install(TARGETS "${PROJECT_NAME}"
#         EXPORT ${PROJECT_NAME}
#         INCLUDES
#         DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
#         # COMPONENT "${SDK_COMPONENT_NAME}" # component is not allowed for includes! Headers are installed separately! Includes only marks the headers for export
#     )

#     file(GLOB_RECURSE HEADERS "${INCLUDES}/*.h" "${INCLUDES}/*.hxx" "${INCLUDES}/*.hpp")
#     foreach(headerFile ${HEADERS})
#         get_filename_component(headerFileParentDir "${headerFile}" DIRECTORY)
#         file(RELATIVE_PATH headerFileRelParentDir "${INCLUDES}" "${headerFileParentDir}")

#         install(FILES "${headerFile}"
#             DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${headerFileRelParentDir}"
#         )
#     endforeach()

#     set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
#     set(CPACK_PACKAGE_DESCRIPTION "${PROJECT_DESCRIPTION}")
#     set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "all")
#     set(CPACK_DEBIAN_PACKAGE_NAME "lib${CPACK_PACKAGE_NAME}-dev")
#     set(CPACK_RPM_PACKAGE_NAME "lib${CPACK_PACKAGE_NAME}-devel")
#     set(CPACK_PACKAGE_HOMEPAGE_URL "${PROJECT_HOMEPAGE_URL}")
#     set(CPACK_PACKAGE_MAINTAINER "${CPACK_PACKAGE_VENDOR}")
#     set(CPACK_DEBIAN_PACKAGE_DEPENDS "")
#     set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${CPACK_PACKAGE_MAINTAINER}")
#     set(CPACK_PACKAGE_MAINTAINER "${CPACK_PACKAGE_VENDOR}")
#     set(CPACK_DEB_COMPONENT_INSTALL ON)
#     set(CPACK_RPM_COMPONENT_INSTALL ON)
#     set(CPACK_NSIS_COMPONENT_INSTALL ON)
#     set(CPACK_DEBIAN_COMPRESSION_TYPE "xz")

#     set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
#     set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

#     set(CMAKE_CONFIG_FILE_BASENAME "${PROJECT_NAME}Config.cmake")
#     set(CMAKE_EXPORT_FILE_BASENAME "${PROJECT_NAME}Export.cmake")
#     set(CMAKE_CONFIG_VERSION_FILE_BASENAME "${PROJECT_NAME}ConfigVersion.cmake")
#     set(CMAKE_CONFIG_VERSION_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CONFIG_VERSION_FILE_BASENAME}")

#     export(TARGETS "${PROJECT_NAME}"
#         NAMESPACE "${EXPORT_NAMESPACE}"
#         FILE "${CMAKE_EXPORT_FILE_BASENAME}"
#         EXPORT_LINK_INTERFACE_LIBRARIES
#     )

#     install(EXPORT "${PROJECT_NAME}"
#         FILE "${CMAKE_CONFIG_FILE_BASENAME}"
#         NAMESPACE "${EXPORT_NAMESPACE}"
#         DESTINATION "${CMAKE_INSTALL_LIBDIR_ARCHIND}/cmake/${PROJECT_NAME}"
#     )

#     write_basic_package_version_file(
#         "${CMAKE_CONFIG_VERSION_FILE_NAME}"
#         #VERSION "100500.100500.100500"  # any version of same bitness suits. CMake cannot compare to infinity, so use a large number we expect to be greater than any future version
#         VERSION ${_VERSION}
#         COMPATIBILITY AnyNewerVersion
#         ARCH_INDEPENDENT
#     )
#     install(FILES "${CMAKE_CONFIG_VERSION_FILE_NAME}"
#         DESTINATION "${CMAKE_INSTALL_LIBDIR_ARCHIND}/cmake/${PROJECT_NAME}"
#     )

#     configure_pkg_config_file("${PROJECT_NAME}"
#         NAME "${PROJECT_NAME}"
#         VERSION "${PROJECT_VERSION}"
#         DESCRIPTION "${CPACK_PACKAGE_DESCRIPTION}"
#         URL "${CPACK_PACKAGE_HOMEPAGE_URL}"
#         INSTALL_LIB_DIR "${CMAKE_INSTALL_LIBDIR_ARCHIND}"
#         INSTALL_INCLUDE_DIR "${CMAKE_INSTALL_INCLUDEDIR}"
#     )

#     include(CPack)
# endif()















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

cmake_minimum_required(VERSION 3.9)

project(std-k VERSION 0.0.1 DESCRIPTION "kidders std library")

add_library(std-k SHARED
    src/std-k.cpp
    inc/std-k.hpp
)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set_target_properties(std-k PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 0
    PUBLIC_HEADER inc/std-k.hpp)

target_include_directories(std-k PRIVATE src, inc)

# configure_file(${PROJECT_SOURCE_DIR}/std-k.pc.in ${PROJECT_SOURCE_DIR}/std-k.pc @ONLY)

install(DIRECTORY inc/std-k.hpp DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(TARGETS std-k
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# install(FILES ${CMAKE_BINARY_DIR}/mylib.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)


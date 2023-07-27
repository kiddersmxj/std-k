cmake_minimum_required(VERSION 3.9)
project(std-k VERSION 0.0.1 DESCRIPTION "kidders std library")

add_library(std-k SHARED
    src/std-k.cpp
    inc/std-k.hpp
)

set_target_properties(std-k PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 0
    PUBLIC_HEADER inc/std-k.hpp
)

# Add include directories to allow other projects to find headers
target_include_directories(std-k PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
    $<INSTALL_INTERFACE:include>
)

# Install the shared library and headers
install(TARGETS std-k
    EXPORT std-k_targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
    PUBLIC_HEADER DESTINATION include
)

install(DIRECTORY inc/ DESTINATION include)

# Generate and install the package configuration files manually
set(ConfigPackageLocation lib/cmake/std-k)

# Generate the package configuration file
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/std-kConfig.cmake" "
include(CMakeFindDependencyMacro)
include(\${CMAKE_CURRENT_LIST_DIR}/std-kTargets.cmake)
")

# Generate the package version file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/std-kConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

# Install the package configuration files
install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/std-kConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/std-kConfigVersion.cmake"
    DESTINATION
        ${ConfigPackageLocation}
)

# Install the export set for other projects to find std-k targets
install(EXPORT std-k_targets
    FILE std-kTargets.cmake
    NAMESPACE std-k::
    DESTINATION ${ConfigPackageLocation}
)

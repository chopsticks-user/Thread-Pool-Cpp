


cmake_minimum_required(VERSION 3.15)

project(Ushi VERSION 1.0.0 LANGUAGES CXX)

include(CMake/Options.cmake)

# Library
include(CMake/CompilerFlags.cmake)

add_library(${Ushi_TARGET_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${Ushi_TARGET_NAME} 
    ALIAS ${Ushi_TARGET_NAME}
)
target_include_directories(${Ushi_TARGET_NAME} INTERFACE
    $<BUILD_INTERFACE:${Ushi_INCLUDE_BUILD_DIR}>
    $<INSTALL_INTERFACE:${Ushi_INCLUDE_INSTALL_DIR}>
)
target_link_libraries(${Ushi_TARGET_NAME} 
    INTERFACE ${PROJECT_NAME}::CompilerFlags
)

# Install
include(CMakePackageConfigHelpers)

install(TARGETS ${Ushi_TARGET_NAME}
        EXPORT ${Ushi_TARGET_NAME}Targets
        PUBLIC_HEADER DESTINATION 
            ${Ushi_INCLUDE_BUILD_DIR}/${Ushi_TARGET_NAME}
)

write_basic_package_version_file("${Ushi_TARGET_NAME}ConfigVersion.cmake"
                                 VERSION ${PROJECT_VERSION}
                                 COMPATIBILITY SameMajorVersion)

configure_package_config_file(
    "${Ushi_CMAKE_MODULE_PATH}/${Ushi_TARGET_NAME}Config.cmake.in"
    "${Ushi_BINARY_DIR}/${Ushi_TARGET_NAME}Config.cmake"
    INSTALL_DESTINATION CMake/${Ushi_TARGET_NAME}
)

# install(DIRECTORY ${PROJECT_SOURCE_DIR}/Source/ 
#     DESTINATION ${PROJECT_SOURCE_DIR}/Include/${PROJECT_NAME}
#     # PATTERN "CMakeLists.txt" EXCLUDE
# )


add_subdirectory(Examples)
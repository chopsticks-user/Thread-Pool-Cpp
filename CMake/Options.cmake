


include(GNUInstallDirs)

option(Ushi_BUILD_EXAMPLES "Build code examples" OFF)
option(Ushi_BUILD_TESTS "Build tests" OFF)

set(Ushi_AS_SUBPROJECT OFF)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(Ushi_AS_SUBPROJECT ON)
endif()

set(Ushi_CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMake)
# include(${Ushi_CMAKE_MODULE_PATH}/Packages.cmake)

set(Ushi_TARGET_NAME ${PROJECT_NAME})
set(Ushi_INCLUDE_BUILD_DIR "${PROJECT_SOURCE_DIR}/Include/")
set(Ushi_INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR})
set(Ushi_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

# set(Container_CONFIG_INSTALL_DIR 
#     "${CMAKE_INSTALL_DATADIR}/cmake/${PROJECT_NAME}" CACHE INTERNAL ""
# )
# set(Container_INCLUDE_INSTALL_DIR       
#     "${CMAKE_INSTALL_INCLUDEDIR}"
# )
# set(Container_TARGETS_EXPORT_NAME       
#     "${PROJECT_NAME}Targets"
# )
# set(Container_CMAKE_CONFIG_TEMPLATE     
#     "cmake/config.cmake.in"
# )
# set(Container_CMAKE_CONFIG_DIR          
#     "${CMAKE_CURRENT_BINARY_DIR}"
# )
# set(Container_CMAKE_VERSION_CONFIG_FILE 
#     "${Container_CMAKE_CONFIG_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
# )
# set(Container_CMAKE_PROJECT_CONFIG_FILE 
#     "${Container_CMAKE_CONFIG_DIR}/${PROJECT_NAME}Config.cmake"
# )
# set(Container_CMAKE_PROJECT_TARGETS_FILE 
#     "${Container_CMAKE_CONFIG_DIR}/${PROJECT_NAME}Targets.cmake"
# )
# set(Container_PKGCONFIG_INSTALL_DIR     
#     "${CMAKE_INSTALL_DATADIR}/pkgconfig"
# )


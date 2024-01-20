


cmake_minimum_required(VERSION 3.15)
project(ThreadPool_Examples LANGUAGES CXX)

if(${CMAKE_CXX_STANDARD} LESS 14)
    return()
endif()

file(GLOB ExamplePaths ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
foreach (ExamplePath ${ExamplePaths})
    get_filename_component(ExampleFileName ${ExamplePath} NAME_WLE)
    add_executable(${ExampleFileName} ${ExamplePath})
    # target_include_directories(${ExampleFileName}
    #     PRIVATE ${Ushi_INCLUDE_DIR}
    # )
    target_link_libraries(${ExampleFileName}
        PRIVATE Ushi::Thread
    )
endforeach (ExamplePath ${ExamplePaths})
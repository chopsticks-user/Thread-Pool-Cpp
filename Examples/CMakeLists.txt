


cmake_minimum_required(VERSION 3.15)
project(TPLExamples LANGUAGES CXX)

find_package(TPL CONFIG REQUIRED PATHS ${CMAKE_BINARY_DIR})

file(GLOB ExamplePaths ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
foreach (ExamplePath ${ExamplePaths})
    get_filename_component(ExampleFileName ${ExamplePath} NAME_WLE)
    add_executable(${ExampleFileName} ${ExamplePath})
    target_include_directories(${ExampleFileName}
        PRIVATE ${TPL_INCLUDE_DIR}
    )
    target_link_libraries(${ExampleFileName}
        PRIVATE CompilerFlags
        PRIVATE TPL
    )
endforeach (ExamplePath ${ExamplePaths})
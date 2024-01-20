


add_library(${PROJECT_NAME}Thread SHARED
    ContractManager.cpp
    WorkerManager.cpp
    ThreadPool.cpp
)

add_library(${PROJECT_NAME}::Thread 
    ALIAS ${Ushi_TARGET_NAME}Thread
)

target_include_directories(${PROJECT_NAME}Thread 
    PUBLIC ${PROJECT_SOURCE_DIR}/Source
)

target_link_libraries(${PROJECT_NAME}Thread 
    INTERFACE ${PROJECT_NAME}::CompilerFlags
)
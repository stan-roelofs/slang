project(xyz-test)
        
set(SOURCES
    src/test.h
    src/test_chunk.c
    src/test_value_array.c
    src/test_vm.c

    src/main.c
)

add_executable(xyz-test ${SOURCES})
target_link_libraries(xyz-test xyz)
target_include_directories(xyz-test PRIVATE ../src)
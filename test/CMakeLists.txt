project(slang-test)
        
set(SOURCES
    src/test.h
    src/test_chunk.c
    src/test_value_array.c
    src/test_util.c
    src/test_vm.c

    src/main.c
)

add_executable(slang-test ${SOURCES})
target_link_libraries(slang-test slang)
target_include_directories(slang-test PRIVATE ../src)
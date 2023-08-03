project(xyz_test)
        
set(SOURCES
    src/test.h
    src/test_chunk.c

    src/main.c
)

add_executable(xyz_test ${SOURCES})
target_link_libraries(xyz_test xyz)
target_include_directories(xyz_test PRIVATE ../src)
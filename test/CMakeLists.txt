project(slang-test)

OPTION(USE_ADDRESS_SANITIZER "Use address sanitizer" OFF)

set(SOURCES
    src/test.h
    src/test_chunk.c
    src/test_scanner.c
    src/test_value_array.c
    src/test_util.c
    src/test_vm.c

    src/main.c
)

add_executable(slang-test ${SOURCES})

if (USE_ADDRESS_SANITIZER)
    target_compile_options(slang-test PRIVATE -fsanitize=address -fno-omit-frame-pointer)
    target_link_options(slang-test PRIVATE -fsanitize=address -fno-omit-frame-pointer)
endif()

target_link_libraries(slang-test slang)
target_include_directories(slang-test PRIVATE ../src)
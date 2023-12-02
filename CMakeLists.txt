cmake_minimum_required(VERSION 3.11)
 
project(slang 
        VERSION 0.1.0 
        DESCRIPTION "A simple but efficient scripting language"
        LANGUAGES C)

option(SLANG_BUILD_TESTS "Build tests" ON)
option(SLANG_BUILD_INTERPRETER "Build interpreter" ON)
option(SLANG_DEBUG_TRACE_EXECUTION "Trace execution" OFF)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
        
set (HEADERS
    include/slang/vm.h

    src/chunk.h
    src/compiler.h
    src/debug.h
    src/memory.h
    src/opcode.h
    src/scanner.h
    src/util.h
    src/value.h
    src/vm_internal.h
)

set(SOURCES
    src/chunk.c
    src/compiler.c
    src/debug.c
    src/memory.c
    src/scanner.c
    src/util.c
    src/value.c
    src/vm.c
)

add_library(slang ${SOURCES} ${HEADERS})
target_include_directories(slang PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

if (SLANG_DEBUG_TRACE_EXECUTION)
    target_compile_definitions(slang PRIVATE -DSLANG_DEBUG)
endif()

if(MSVC)
  target_compile_options(slang PRIVATE /W4 /WX)
  target_compile_definitions(slang PRIVATE -D_CRT_SECURE_NO_WARNINGS)
else()
  target_compile_options(slang PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

if (SLANG_BUILD_INTERPRETER)
    add_subdirectory(interpreter)
endif()

if (SLANG_BUILD_TESTS)
    add_subdirectory(test)
endif()
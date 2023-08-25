cmake_minimum_required(VERSION 3.11)
 
project(xyz 
        VERSION 0.1.0 
        DESCRIPTION "A simple but efficient scripting language"
        LANGUAGES C)

option(XYZ_BUILD_TESTS "Build tests" OFF)
option(XYZ_BUILD_INTERPRETER "Build interpreter" ON)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
        
set (HEADERS
    src/chunk.h
    src/common.h
    src/debug.h
    src/memory.h
    src/opcode.h
    src/value.h
    src/vm.h
)

set(SOURCES
    src/chunk.c
    src/debug.c
    src/memory.c
    src/value.c
    src/vm.c
)

add_library(xyz ${SOURCES} ${HEADERS})
target_include_directories(xyz PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

if (XYZ_BUILD_INTERPRETER)
    add_executable(xyz-interpreter src/main.c)
    target_link_libraries(xyz-interpreter xyz)
    set_target_properties(xyz-interpreter PROPERTIES OUTPUT_NAME "xyz")
endif()

if (XYZ_BUILD_TESTS)
    add_subdirectory(test)
endif()
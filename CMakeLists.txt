cmake_minimum_required(VERSION 3.11)
 
project(xyz 
        VERSION 0.1.0 
        DESCRIPTION "A simple but efficient scripting language"
        LANGUAGES C)
        
set (HEADERS
)

set(SOURCES
    src/main.c
)

add_executable(xyz ${SOURCES} ${HEADERS})
target_include_directories(xyz PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

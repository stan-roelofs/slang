project(slang_interpreter)        
    
add_executable(slang-interpreter main.c)
target_link_libraries(slang-interpreter slang)
set_target_properties(slang-interpreter PROPERTIES OUTPUT_NAME "slang")
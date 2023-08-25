project(xyz_interpreter)        
    
add_executable(xyz-interpreter main.c)
target_link_libraries(xyz-interpreter xyz)
set_target_properties(xyz-interpreter PROPERTIES OUTPUT_NAME "xyz")
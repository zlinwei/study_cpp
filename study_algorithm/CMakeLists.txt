aux_source_directory(. SRCS)

#for each source: build an executable and install
foreach(example_source ${SRCS})
    get_filename_component(example_name ${example_source} NAME_WE)
    add_executable(${example_name} ${example_source})
    message(STATUS "add executable: ${example_name} fom source: ${example_source}")
endforeach(example_source)
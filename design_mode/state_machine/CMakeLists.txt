find_package(OpenCV REQUIRED)
aux_source_directory(. SRCS)

#for each source: build an executable and install
foreach(example_source ${SRCS})
    get_filename_component(example_name ${example_source} NAME_WE)
    add_executable(${example_name} ${example_source})
    target_link_libraries(${example_name} ${Boost_LIBRARIES}  ${OpenCV_LIBS} glog::glog)
    target_include_directories(${example_name} PRIVATE ${OpenCV_INCLUDE_DIRS})
    message(STATUS "add executable: ${example_name} fom source: ${example_source}")
endforeach(example_source)
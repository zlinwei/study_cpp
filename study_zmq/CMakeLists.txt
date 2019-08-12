find_package(cppzmq CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

aux_source_directory(. SRCS)
#for each source: build an executable and install
foreach (example_source ${SRCS})
    get_filename_component(example_name ${example_source} NAME_WE)
    add_executable(${example_name} ${example_source})
    target_link_libraries(${example_name} ${Boost_LIBRARIES} cppzmq glog::glog nlohmann_json nlohmann_json::nlohmann_json)
    message(STATUS "add executable: ${example_name} fom source: ${example_source}")
endforeach (example_source)
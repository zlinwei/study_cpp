aux_source_directory(. SRCS)
#message(STATUS "${Boost_LIBRARIES}")
add_library(my_plugin_api SHARED my_plugin_api.cpp)
target_link_libraries(my_plugin_api ${Boost_LIBRARIES})

add_executable(my_plugin_api_test my_plugin_api_test.cpp)
target_link_libraries(my_plugin_api_test ${Boost_LIBRARIES})



add_library(plugin_calculator SHARED plugin_calculator.cpp)
target_link_libraries(plugin_calculator ${Boost_LIBRARIES})

add_executable(plugin_calculator_test plugin_calculator_test.cpp)
target_link_libraries(plugin_calculator_test ${Boost_LIBRARIES})

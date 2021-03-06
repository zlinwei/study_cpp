cmake_minimum_required(VERSION 3.14)
project(study_cpp)

set(CMAKE_CXX_STANDARD 14)

# BOOST
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
set(BOOST_ALL_DYN_LINK ON)
SET(BOOST_REQUIRED_COMPONENTS
        date_time
        filesystem
        program_options
        regex
        system
        thread
        #        python3.7
        )
set(BOOST_MIN_VERSION "1.58")
set(Boost_ADDITIONAL_VERSIONS
        "1.56.0" "1.56" "1.57" "1.57" "1.58" "1.59" "1.60" "1.61" "1.62" "1.63"
        "1.64" "1.65" "1.66" "1.67" "1.68" "1.69" "1.70"
        )
find_package(Boost ${BOOST_MIN_VERSION} COMPONENTS ${BOOST_REQUIRED_COMPONENTS})
message(STATUS "** Boost Include: ${Boost_INCLUDE_DIR}")
message(STATUS "** Boost Libraries: ${Boost_LIBRARY_DIRS}")
message(STATUS "** Boost Libraries: ${Boost_LIBRARIES}")
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR}/python3.7)
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
# glog
find_package(glog CONFIG REQUIRED)

add_subdirectory(study_boost_asio)
add_subdirectory(study_boost_compute)
add_subdirectory(study_opencv)
add_subdirectory(study_boost_python)
add_subdirectory(study_pytorch)
add_subdirectory(design_mode)
add_subdirectory(study_algorithm)
#add_subdirectory(study_libjpeg_turbo)
add_subdirectory(study_libuv)
add_subdirectory(study_zmq)
add_executable(study_cpp main.cpp)
add_subdirectory(study_trick)
#add_subdirectory(study_grpc)
add_subdirectory(study_boost_dll)
add_subdirectory(study_fbs)

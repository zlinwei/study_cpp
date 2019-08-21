find_program(GRPC_CPP_PLUGIN grpc_cpp_plugin) # Get full path to plugin
message(STATUS "${GRPC_CPP_PLUGIN}")
function(PROTOBUF_GENERATE_GRPC_CPP SRCS HDRS)
    if (NOT ARGN)
        message(SEND_ERROR "Error: PROTOBUF_GENERATE_GRPC_CPP() called without any proto files")
        return()
    endif ()

    if (PROTOBUF_GENERATE_CPP_APPEND_PATH) # This variable is common for all types of output.
        # Create an include path for each file specified
        foreach (FIL ${ARGN})
            get_filename_component(ABS_FIL ${FIL} ABSOLUTE)
            get_filename_component(ABS_PATH ${ABS_FIL} PATH)
            list(FIND _protobuf_include_path ${ABS_PATH} _contains_already)
            if (${_contains_already} EQUAL -1)
                list(APPEND _protobuf_include_path -I ${ABS_PATH})
            endif ()
        endforeach ()
    else ()
        set(_protobuf_include_path -I ${CMAKE_CURRENT_SOURCE_DIR})
    endif ()

    if (DEFINED PROTOBUF_IMPORT_DIRS)
        foreach (DIR ${Protobuf_IMPORT_DIRS})
            get_filename_component(ABS_PATH ${DIR} ABSOLUTE)
            list(FIND _protobuf_include_path ${ABS_PATH} _contains_already)
            if (${_contains_already} EQUAL -1)
                list(APPEND _protobuf_include_path -I ${ABS_PATH})
            endif ()
        endforeach ()
    endif ()

    set(${SRCS})
    set(${HDRS})
    foreach (FIL ${ARGN})
        get_filename_component(ABS_FIL ${FIL} ABSOLUTE)
        get_filename_component(FIL_WE ${FIL} NAME_WE)

        list(APPEND ${SRCS} "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.grpc.pb.cc")
        list(APPEND ${HDRS} "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.grpc.pb.h")

        add_custom_command(
                OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.grpc.pb.cc"
                "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.grpc.pb.h"
                COMMAND ${Protobuf_PROTOC_EXECUTABLE}
                ARGS --grpc_out=${CMAKE_CURRENT_BINARY_DIR}
                --plugin=protoc-gen-grpc=${GRPC_CPP_PLUGIN}
                ${_protobuf_include_path} ${ABS_FIL}
                DEPENDS ${ABS_FIL} ${Protobuf_PROTOC_EXECUTABLE}
                COMMENT "Running gRPC C++ protocol buffer compiler on ${FIL}"
                VERBATIM)
    endforeach ()

    set_source_files_properties(${${SRCS}} ${${HDRS}} PROPERTIES GENERATED TRUE)
    set(${SRCS} ${${SRCS}} PARENT_SCOPE)
    set(${HDRS} ${${HDRS}} PARENT_SCOPE)
endfunction()


find_package(Protobuf REQUIRED)
find_package(gRPC)

include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
#protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS greeter.proto)
#protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS EXPORT_MACRO DLL_EXPORT greeter.proto)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS greeter.proto)

file(GLOB ProtoFiles "${CMAKE_CURRENT_SOURCE_DIR}/*.proto")
PROTOBUF_GENERATE_GRPC_CPP(ProtoGRPCSources ProtoGRPCHeaders ${ProtoFiles})

message(STATUS "current binary dir: ${CMAKE_CURRENT_BINARY_DIR}")

include_directories(${CMAKE_CURRENT_BINARY_DIR})


message(STATUS "${ProtoGRPCSources}")
message(STATUS "${ProtoGRPCHeaders}")
message(STATUS "${PROTO_SRCS}")
message(STATUS "${PROTO_HDRS}")
add_library(greeter_pb ${PROTO_SRCS})
target_link_libraries(greeter_pb protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite gRPC::gpr gRPC::grpc gRPC::grpc++ gRPC::grpc_cronet)

add_library(greeter_rpc ${ProtoGRPCSources})
target_link_libraries(greeter_rpc greeter_pb)

find_package(gRPC)

aux_source_directory(. SRCS)
#for each source: build an executable and install
foreach (example_source ${SRCS})
    get_filename_component(example_name ${example_source} NAME_WE)
    add_executable(${example_name} ${example_source})
    target_link_libraries(${example_name} ${Boost_LIBRARIES} ${Protobuf_LIBRARIES} greeter_rpc)
    message(STATUS "add executable: ${example_name} fom source: ${example_source}")
endforeach (example_source)
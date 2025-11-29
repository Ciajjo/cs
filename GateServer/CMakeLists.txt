project(gateserver)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

file(GLOB MAINSRC ${PROJECT_SOURCE_DIR}/*.cpp)
file(GLOB SRC ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB CCSRC ${PROJECT_SOURCE_DIR}/src/*.cc)

#生成动态库|静态库
add_library(Gate SHARED ${SRC} ${CCSRC})

set(Protobuf_ROOT /usr)
# 查找 Protobuf（必须放在 gRPC 之前）
find_package(Protobuf REQUIRED)
# 然后查找 gRPC
find_package(gRPC REQUIRED)

find_package(Boost REQUIRED COMPONENTS filesystem system)

add_executable(gateserver ${MAINSRC})
target_link_libraries(gateserver Gate
    gRPC::grpc++
    gRPC::grpc
    protobuf::libprotobuf
    Boost::filesystem
    Boost::system
    hiredis
    mysqlcppconn
)

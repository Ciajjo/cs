cmake_minimum_required(VERSION 3.15)
project(cahtserver)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_BUILD_TYPE "Debug")  
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")  

#寻找头文件和库的目录
include_directories(${PROJECT_SOURCE_DIR}/GateServer/inc)
# link_directories(${PROJECT_SOURCE_DIR}/lib)





# 添加子目录
add_subdirectory(GateServer)
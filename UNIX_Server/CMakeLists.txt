cmake_minimum_required(VERSION 3.10)

project(PesterServer)

# C++ VERSION
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# directories of sources and headers
include_directories(${PROJECT_SOURCE_DIR}/DataBase /usr/include
                    /usr/include/cppconn)

set(SOURCES main.cpp server.cpp DataBase/ChatDatabase.cpp)

# creating an executable file
add_executable(PesterServer ${SOURCES})

# linking libraries
target_link_libraries(PesterServer PRIVATE mysqlcppconn)

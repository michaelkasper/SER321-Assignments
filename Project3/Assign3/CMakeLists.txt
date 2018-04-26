cmake_minimum_required(VERSION 3.8)
project(Waypoint)

#set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

set(SOURCE_DIR ./src/sample/cpp)

link_directories(/opt/local/lib)
link_directories(/usr/lib)

include_directories(${SOURCE_DIR})
include_directories(/opt/local/include)

add_executable(waypoint ${SOURCE_DIR}/WaypointApp.cpp ${SOURCE_DIR}/WaypointGUI.cpp ${SOURCE_DIR}/Waypoint.cpp)
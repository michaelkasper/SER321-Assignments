cmake_minimum_required(VERSION 3.8)
project(Waypoint)

#set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

set(SOURCE_DIR ./server)
#set(SOURCE_DIR ./client)

link_directories(/opt/local/lib)
link_directories(/usr/lib)

include_directories(${SOURCE_DIR})
#include_directories(./server)
include_directories(/opt/local/include)

add_executable(waypointServer ${SOURCE_DIR}/Waypoint.cpp ${SOURCE_DIR}/WaypointCollection.cpp ${SOURCE_DIR}/WaypointServer.cpp)
#add_executable(waypointClient ${SOURCE_DIR}/../server/Waypoint.cpp ${SOURCE_DIR}/WaypointApp.cpp ${SOURCE_DIR}/WaypointGUI.cpp client/waypointcollectionstub.h)
cmake_minimum_required(VERSION 3.17)
project(RM_LinkGame)
project(test)

set(CMAKE_CXX_STANDARD 17)

#######OpenCV##########
find_package(OpenCV 4 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIBRARY_DIRS})
########################

FILE(GLOB HeaderFiles "*.h")

add_executable(RM_LinkGame main.cpp ${HeaderFiles})
add_executable(test test.cpp ${HeaderFiles})

target_link_libraries(RM_LinkGame ${OpenCV_LIBS})
target_link_libraries(test ${OpenCV_LIBS})
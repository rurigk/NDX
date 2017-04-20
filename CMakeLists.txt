project(PanelToolsNoQT)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 11) # C++11...
set(CMAKE_CXX_STANDARD_REQUIRED ON) #...is required...
set(CMAKE_CXX_EXTENSIONS OFF) #...without compiler extensions like gnu++11

include_directories(include)
FILE(GLOB_RECURSE cppfiles "include/*.cpp")
FILE(GLOB_RECURSE hppfiles "include/*.hpp")
FILE(GLOB_RECURSE hfiles "include/*.h")

set(SRC_LIST ${cppfiles} ${hppfiles} ${hfiles})

add_custom_target(headers SOURCES ${SRC_LIST})

aux_source_directory(. SRC_LIST)

list(REMOVE_ITEM SRC_LIST "./NDX.cc")
add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} X11)

cmake_minimum_required(VERSION 3.7.0)

project(MySQL)

set (CMAKE_C_FLAGS "-D_GNU_SOURCE -Wall -std=c99")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_BUILD_TYPE Debug)


#build application
add_executable(application "")

target_link_libraries(application PUBLIC hardware)

target_sources(application
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/src/main.c
  )

target_include_directories(application 
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/lib  
  )

install(TARGETS application DESTINATION bin)

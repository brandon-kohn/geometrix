CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)
PROJECT(geometrix)

add_library(geometrix INTERFACE)
target_include_directories(geometrix INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/geometrix>
  $<INSTALL_INTERFACE:include/geometrix>
)

install ( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/geometrix/ DESTINATION ${CMAKE_INSTALL_PREFIX}/include/geometrix)
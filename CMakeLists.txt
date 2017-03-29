cmake_minimum_required(VERSION 3.0.2)

# Build Properties
set(CMAKE_INSTALL_INCLUDEDIR "include" CACHE PATH "Default relative include path.")

# Targets
project(geometrix)

add_library(geometrix INTERFACE)
target_include_directories(geometrix INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/geometrix>
  $<INSTALL_INTERFACE:include>
)

# Deployment
install ( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/geometrix/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/geometrix)
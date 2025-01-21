cmake_minimum_required(VERSION 3.1)
project(labor)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".so")
set(CMAKE_EXE_LINKER_FLAGS "-flto")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O2 --std=c++17 -Wall -flto" )

enable_language(CXX)

add_executable(labor src/labor.cpp)

set(LIBS ${LIBS})

target_include_directories(labor PRIVATE
  include
  modules/tclap/include
  modules/simde/include)
target_link_libraries(labor ${LIBS})

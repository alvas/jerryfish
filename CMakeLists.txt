cmake_minimum_required(VERSION 3.0)
project(JerryFish)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -O0 -fPIC -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")

include_directories(.)

find_package(Boost REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

link_directories(/usr/local/lib)

set(LIB_SRC
    src/Logger.cc
    src/Utils.cc
    )

add_library(JerryFish SHARED ${LIB_SRC})
add_library(JerryFish_static STATIC ${LIB_SRC})
#set_target_properties(JerryFish_static PROPERTIES LINKER_LANGUAGE CXX OUTPUT_NAME "JerryFish")
set_target_properties(JerryFish PROPERTIES LINKER_LANGUAGE CXX OUTPUT_NAME "JerryFish")

add_executable(test tests/test.cc)
add_dependencies(test JerryFish)
target_link_libraries(test JerryFish yaml-cpp)

#find_library(YAMLCPP yaml-cpp)

add_executable(test_config tests/test_config.cc)
add_dependencies(test_config JerryFish)
target_link_libraries(test_config JerryFish yaml-cpp)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

## Download and unpack googletest at configure time
#configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
#execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  #RESULT_VARIABLE result
  #WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
#if(result)
  #message(FATAL_ERROR "CMake step for googletest failed: ${result}")
#endif()
#execute_process(COMMAND ${CMAKE_COMMAND} --build .
  #RESULT_VARIABLE result
  #WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
#if(result)
  #message(FATAL_ERROR "Build step for googletest failed: ${result}")
#endif()

## Prevent overriding the parent project's compiler/linker
## settings on Windows
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

## Add googletest directly to our build. This defines
## the gtest and gtest_main targets.
#add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 #${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 #EXCLUDE_FROM_ALL)

## The gtest/gtest_main targets carry header search path
## dependencies automatically when using CMake 2.8.11 or
## later. Otherwise we have to add them here ourselves.
#if (CMAKE_VERSION VERSION_LESS 2.8.11)
  #include_directories("${gtest_SOURCE_DIR}/include")
#endif()

## Now simply link against gtest or gtest_main as needed. Eg
#add_executable(example example.cpp)
#target_link_libraries(example gtest_main)
#add_test(NAME example_test COMMAND example)

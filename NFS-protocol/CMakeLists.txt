cmake_minimum_required(VERSION 3.13)

project(NFS)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set (CMAKE_POSITION_INDEPENDENT_CODE TRUE)


file(GLOB SOURCES_LIBCORE "libraries/core/*.cpp" "libraries/core/*.hpp")
message( "${SOURCES_LIBCORE}")
add_library(libcore ${SOURCES_LIBCORE})

file(GLOB SOURCES_LIBCLIENT "libraries/client/*.cpp" "libraries/client/*.hpp")
add_library(libclient ${SOURCES_LIBCLIENT})

file(GLOB SOURCES_SERVER "server/*.cpp" "server/*.hpp")
find_package( Threads )
add_executable(server ${SOURCES_SERVER} server/DescriptorsMapper.h server/handler.cpp server/handler.h)
target_link_libraries(server PUBLIC libcore ${CMAKE_THREAD_LIBS_INIT})


file(GLOB SOURCES_CLIENT "client/*.cpp" "client/*.hpp" "client/*.h" "client/command_handlers/*.h" "client/command_handlers/*.cpp" "client/include/linenoise/linenoise/*.h" "client/include/linenoise/linenoise/linenoise.c")

add_executable(client ${SOURCES_CLIENT})
target_link_libraries(client PUBLIC stdc++fs)
target_link_libraries(libclient PUBLIC libcore)
target_link_libraries(client PUBLIC libclient)

MESSAGE("adding tests")

file(GLOB TESTS "tests/*.cpp" "client/tests/*.cpp")

include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.10.0
)

FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
  FetchContent_Populate(googletest)
  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()


file(GLOB HANDLERS "client/command_handlers/*")



foreach(TEST ${TESTS})
    MESSAGE("adding ${TEST}")
    get_filename_component(NAME "test_${TEST}" NAME_WE)
    set(NAME "test_${NAME}")
    add_executable(${NAME} ${TEST} ${HANDLERS})
    
    target_link_libraries(${NAME} PUBLIC libclient gtest gmock gtest_main libcore)
endforeach()

  
file(GLOB PERF_TESTS "performance_tests/*")

foreach(TEST ${PERF_TESTS})
    MESSAGE("adding ${TEST}")
    get_filename_component(NAME "perftest_${TEST}" NAME_WE)
    set(NAME "perftest_${NAME}")
    add_executable(${NAME} ${TEST} ${HANDLERS})
    
    target_link_libraries(${NAME} PUBLIC libclient libcore)
  

endforeach()

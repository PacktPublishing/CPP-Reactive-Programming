# to run the tests: cmake, then make, then ctest

set(BOOST_ROOT "${CMAKE_CURRENT_SOURCE_DIR}\\..\\..\\boost_1_58_0")
set(BOOSTROOT "${CMAKE_CURRENT_SOURCE_DIR}\\..\\..\\boost_1_58_0")

find_package(Boost 1.58.0)

if(NOT Boost_FOUND)
    message(FATAL_ERROR "Could not find boost!")
endif()

include_directories(${Boost_INCLUDE_DIRS})

include_directories("../src/")
add_definitions(-DUSE_MAIN -U__STRICT_ANSI__)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(UNITTEST CXX)
SET(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wa,-mbig-ob")

ADD_CUSTOM_TARGET(alltests ALL)

FILE(GLOB_RECURSE testFiles RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
        "${CMAKE_CURRENT_SOURCE_DIR}/*test.cpp")

FOREACH (testfile ${testFiles})
    GET_FILENAME_COMPONENT(testname ${testfile} NAME_WE)
    MESSAGE(STATUS "Process test: ${testname}")

    ADD_EXECUTABLE(${testname} ${testfile})
    TARGET_LINK_LIBRARIES(${testname} ${Boost_LIBRARIES})

    ADD_TEST(NAME ${testname} COMMAND ${testname})
    ADD_DEPENDENCIES(alltests ${testname})
ENDFOREACH (testfile)


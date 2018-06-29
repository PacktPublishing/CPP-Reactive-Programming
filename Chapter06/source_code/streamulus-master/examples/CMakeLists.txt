CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(EXAMPLES CXX)

add_executable(example_a A_hello_world.cpp)
add_executable(example_b B_common_subexpression.cpp)
add_executable(example_c C_user_defined_functions_and_filters.cpp)
add_executable(example_d D_sliding_window_function.cpp)

target_link_libraries(example_a ${Boost_LIBRARIES})
target_link_libraries(example_b ${Boost_LIBRARIES})
target_link_libraries(example_c ${Boost_LIBRARIES})
target_link_libraries(example_d ${Boost_LIBRARIES})

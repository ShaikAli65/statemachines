cmake_minimum_required(VERSION 3.16)

project(statemachines LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

add_library(statemachines
    src/stm/dfa.cpp
    src/stm/input.cpp
    src/stm/linear_bound.cpp
    src/stm/logger.cpp
    src/stm/nfa.cpp
    src/stm/push_down.cpp
    src/stm/states.cpp
    src/stm/turing.cpp
    src/stm/utilities.cpp
)

target_include_directories(statemachines PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(statemachines_example main.cpp)
target_link_libraries(statemachines_example PRIVATE statemachines)

add_subdirectory(tests)

add_executable(statemachines_tests
    logger_tests.cpp
    parser_tests.cpp
    test_main.cpp
    transform_tests.cpp
)

target_link_libraries(statemachines_tests PRIVATE statemachines)

add_test(NAME statemachines_tests COMMAND statemachines_tests)

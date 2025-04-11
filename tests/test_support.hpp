#pragma once

#include <stdexcept>
#include <string>

struct test_failure : std::runtime_error {
    using std::runtime_error::runtime_error;
};

inline void expect(bool condition, const std::string& message) {
    if (!condition) {
        throw test_failure(message);
    }
}

inline void expect_equal(
    const std::string& actual,
    const std::string& expected,
    const std::string& label) {
    if (actual != expected) {
        throw test_failure(label + ": expected '" + expected + "', got '" + actual + "'");
    }
}

inline void expect_equal(std::size_t actual, std::size_t expected, const std::string& label) {
    if (actual != expected) {
        throw test_failure(
            label + ": expected " + std::to_string(expected) + ", got " + std::to_string(actual));
    }
}

inline std::string sample_nfa_text() {
    return R"(states: A B C
inputs: 0 1
start: A
finals: C
transitions:
A 0 A
A 1 A B
B 1 C
end
)";
}

void test_parse_nfa_spec();
void test_parse_and_build_dfa();
void test_build_nfa_and_transform_to_dfa();
void test_validate_string_accepts_and_rejects();
void test_logger_captures_messages();

#include "test_support.hpp"

#include <functional>
#include <stdexcept>
#include <utility>
#include <vector>

int main() {
    const std::vector<std::pair<const char*, std::function<void()>>> tests = {
        {"parse_nfa_spec", test_parse_nfa_spec},
        {"parse_and_build_dfa", test_parse_and_build_dfa},
        {"build_nfa_and_transform_to_dfa", test_build_nfa_and_transform_to_dfa},
        {"validate_string_accepts_and_rejects", test_validate_string_accepts_and_rejects},
        {"logger_captures_messages", test_logger_captures_messages},
    };

    std::size_t passed = 0;
    for (const auto& [name, test] : tests) {
        try {
            test();
            ++passed;
        } catch (const std::exception& error) {
            throw std::runtime_error(std::string("test '") + name + "' failed: " + error.what());
        }
    }

    expect_equal(passed, tests.size(), "passed test count");
    return 0;
}

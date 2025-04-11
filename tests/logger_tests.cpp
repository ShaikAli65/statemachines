#include "test_support.hpp"

#include "stm/fsm.h"

#include <sstream>

void test_logger_captures_messages() {
    std::ostringstream output;
    stm::logger log(output, stm::log_level::debug);

    std::istringstream input(sample_nfa_text());
    const stm::nfa machine = stm::build_nfa(stm::parse_nfa(input), &log);
    const stm::dfa converted = stm::transform(machine, &log);
    const bool accepted = stm::validate_string("11", converted, &log);

    expect(accepted, "logged validation should accept 11");

    const std::string logs = output.str();
    expect(logs.find("[INFO] building nfa from parsed specification") != std::string::npos,
           "logs should include build message");
    expect(logs.find("[INFO] starting nfa -> dfa transform") != std::string::npos,
           "logs should include transform message");
    expect(logs.find("[INFO] validating input string '11'") != std::string::npos,
           "logs should include validation message");
}

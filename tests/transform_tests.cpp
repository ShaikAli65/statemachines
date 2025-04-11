#include "test_support.hpp"

#include "stm/fsm.h"

#include <sstream>

void test_build_nfa_and_transform_to_dfa() {
    std::istringstream input(sample_nfa_text());
    const stm::nfa_spec spec = stm::parse_nfa(input);
    const stm::nfa machine = stm::build_nfa(spec);
    const stm::dfa converted = stm::transform(machine);

    expect(machine.start_state() != nullptr, "nfa should have a start state");
    expect_equal(machine.start_state()->name, "A", "nfa start state name");
    expect(machine.find_state("C") != nullptr && machine.find_state("C")->is_final,
           "nfa final state C should exist");

    expect(converted.start_state() != nullptr, "dfa should have a start state");
    expect_equal(converted.start_state()->name, "A", "dfa start state name");
    expect(converted.find_state("AB") != nullptr, "converted dfa should contain mixed state AB");
    expect(converted.find_state("ABC") != nullptr, "converted dfa should contain mixed state ABC");
    expect(converted.find_state("ABC")->is_final, "mixed state ABC should be final");
}

void test_validate_string_accepts_and_rejects() {
    std::istringstream input(sample_nfa_text());
    const stm::dfa converted = stm::transform(stm::build_nfa(stm::parse_nfa(input)));

    expect(stm::validate_string("11", converted), "11 should be accepted");
    expect(stm::validate_string("111", converted), "111 should be accepted");
    expect(!stm::validate_string("10", converted), "10 should be rejected");
    expect(!stm::validate_string("2", converted), "unknown symbol should be rejected");
}

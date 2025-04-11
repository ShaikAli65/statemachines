#include "test_support.hpp"

#include "stm/fsm.h"

#include <sstream>

void test_parse_nfa_spec() {
    std::istringstream input(sample_nfa_text());
    const stm::nfa_spec spec = stm::parse_nfa(input);

    expect_equal(spec.states.size(), 3, "parsed state count");
    expect_equal(spec.inputs.size(), 2, "parsed input count");
    expect_equal(spec.start_state, "A", "parsed start state");
    expect_equal(spec.final_states.size(), 1, "parsed final state count");
    expect_equal(spec.transitions.size(), 3, "parsed transition count");
    expect_equal(spec.transitions[1].from, "A", "second transition source");
    expect(spec.transitions[1].input == '1', "second transition input should be '1'");
    expect_equal(spec.transitions[1].to.size(), 2, "second transition destination count");
}

void test_parse_and_build_dfa() {
    std::istringstream input(R"(states: S A
inputs: 0 1
start: S
finals: A
transitions:
S 0 S
S 1 A
A 0 S
A 1 A
end
)");

    const stm::dfa machine = stm::build_dfa(stm::parse_dfa(input));
    expect(machine.start_state() != nullptr, "parsed dfa should have a start state");
    expect_equal(machine.start_state()->name, "S", "parsed dfa start state");
    expect(machine.find_state("A") != nullptr && machine.find_state("A")->is_final,
           "parsed dfa should have final state A");
    expect(stm::validate_string("1", machine), "dfa should accept 1");
    expect(!stm::validate_string("0", machine), "dfa should reject 0");
}

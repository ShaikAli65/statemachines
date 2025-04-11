#include "states.h"

namespace stm {

nfa_state::nfa_state(std::string state_name) : name(std::move(state_name)) {
}

void nfa_state::add_transition(char input, const std::shared_ptr<nfa_state>& next) {
    transitions[input].push_back(next);
}

std::vector<std::shared_ptr<nfa_state>> nfa_state::transitions_for(char input) const {
    const auto it = transitions.find(input);
    return it == transitions.end() ? std::vector<std::shared_ptr<nfa_state>>{} : it->second;
}

dfa_state::dfa_state(std::string state_name) : name(std::move(state_name)) {
}

void dfa_state::set_transition(char input, const std::shared_ptr<dfa_state>& next) {
    transitions[input] = next;
}

std::shared_ptr<dfa_state> dfa_state::transition_for(char input) const {
    const auto it = transitions.find(input);
    return it == transitions.end() ? nullptr : it->second;
}

}  // namespace stm

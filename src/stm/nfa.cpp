#include "nfa.hpp"

#include <algorithm>

namespace stm {

std::shared_ptr<nfa_state> nfa::add_state(const std::string& name, bool is_final) {
    auto state = ensure_state(name);
    state->is_final = state->is_final || is_final;

    if (state->is_final &&
        std::find(final_states_.begin(), final_states_.end(), state) == final_states_.end()) {
        final_states_.push_back(state);
    }

    return state;
}

void nfa::set_start_state(const std::string& name) {
    start_state_ = ensure_state(name);
}

void nfa::add_input(char input) {
    if (!has_input(input)) {
        input_signals_.push_back(input);
    }
}

void nfa::add_transition(const std::string& from, char input, const std::string& to) {
    add_input(input);
    auto from_state = ensure_state(from);
    auto to_state = ensure_state(to);
    from_state->add_transition(input, to_state);
}

bool nfa::has_state(const std::string& name) const {
    return states_.find(name) != states_.end();
}

std::shared_ptr<nfa_state> nfa::find_state(const std::string& name) const {
    const auto it = states_.find(name);
    return it == states_.end() ? nullptr : it->second;
}

const std::shared_ptr<nfa_state>& nfa::start_state() const {
    return start_state_;
}

const std::unordered_map<std::string, std::shared_ptr<nfa_state>>& nfa::states() const {
    return states_;
}

const std::vector<std::shared_ptr<nfa_state>>& nfa::final_states() const {
    return final_states_;
}

const std::vector<char>& nfa::input_signals() const {
    return input_signals_;
}

std::shared_ptr<nfa_state> nfa::ensure_state(const std::string& name) {
    const auto it = states_.find(name);
    if (it != states_.end()) {
        return it->second;
    }

    auto state = std::make_shared<nfa_state>(name);
    states_.emplace(name, state);
    return state;
}

bool nfa::has_input(char input) const {
    return std::find(input_signals_.begin(), input_signals_.end(), input) != input_signals_.end();
}

}  // namespace stm

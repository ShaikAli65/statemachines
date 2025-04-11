#include "dfa.hpp"

#include <algorithm>
#include <stdexcept>

namespace stm {

std::shared_ptr<dfa_state> dfa::add_state(const std::string& name, bool is_final) {
    auto state = ensure_state(name);
    state->is_final = state->is_final || is_final;

    if (state->is_final &&
        std::find(final_states_.begin(), final_states_.end(), state) == final_states_.end()) {
        final_states_.push_back(state);
    }

    return state;
}

void dfa::set_start_state(const std::string& name) {
    start_state_ = ensure_state(name);
}

void dfa::add_input(char input) {
    if (!has_input(input)) {
        input_signals_.push_back(input);
    }
}

void dfa::set_transition(const std::string& from, char input, const std::string& to) {
    add_input(input);
    auto from_state = ensure_state(from);
    auto to_state = ensure_state(to);
    from_state->set_transition(input, to_state);
}

bool dfa::has_state(const std::string& name) const {
    return states_.find(name) != states_.end();
}

std::shared_ptr<dfa_state> dfa::find_state(const std::string& name) const {
    const auto it = states_.find(name);
    return it == states_.end() ? nullptr : it->second;
}

bool dfa::accepts(const std::string& input, logger* log) const {
    if (!start_state_) {
        throw std::logic_error("dfa start state is not set");
    }

    auto current = start_state_;
    if (log) {
        log->debug("dfa validation start at state '" + current->name + "'");
    }

    for (char symbol : input) {
        if (!has_input(symbol)) {
            if (log) {
                log->warn(std::string("dfa rejected unknown input symbol '") + symbol + "'");
            }
            return false;
        }

        if (log) {
            log->debug(
                "dfa transition from '" + current->name + "' on '" + std::string(1, symbol) + "'");
        }
        current = current->transition_for(symbol);
        if (!current || current->name == "_") {
            if (log) {
                log->info("dfa transition missing, input rejected");
            }
            return false;
        }
    }

    if (log) {
        log->info(
            "dfa validation ended at '" + current->name +
            std::string(current->is_final ? "' (accepted)" : "' (rejected)"));
    }
    return current->is_final;
}

const std::shared_ptr<dfa_state>& dfa::start_state() const {
    return start_state_;
}

const std::unordered_map<std::string, std::shared_ptr<dfa_state>>& dfa::states() const {
    return states_;
}

const std::vector<std::shared_ptr<dfa_state>>& dfa::final_states() const {
    return final_states_;
}

const std::vector<char>& dfa::input_signals() const {
    return input_signals_;
}

std::shared_ptr<dfa_state> dfa::ensure_state(const std::string& name) {
    const auto it = states_.find(name);
    if (it != states_.end()) {
        return it->second;
    }

    auto state = std::make_shared<dfa_state>(name);
    states_.emplace(name, state);
    return state;
}

bool dfa::has_input(char input) const {
    return std::find(input_signals_.begin(), input_signals_.end(), input) != input_signals_.end();
}

}  // namespace stm

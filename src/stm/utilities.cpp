#include "fsm.h"

#include <algorithm>
#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

namespace stm {
namespace {

std::string mixed_state_name(const std::set<std::string>& state_names) {
    if (state_names.empty()) {
        return "_";
    }

    std::string name;
    for (const auto& state_name : state_names) {
        name += state_name;
    }
    return name;
}

bool mixed_state_is_final(const std::set<std::string>& state_names, const nfa& machine) {
    for (const auto& state_name : state_names) {
        const auto state = machine.find_state(state_name);
        if (state && state->is_final) {
            return true;
        }
    }
    return false;
}

std::set<std::string> next_mixed_state(
    const std::set<std::string>& current,
    char input,
    const nfa& machine) {
    std::set<std::string> next;

    for (const auto& state_name : current) {
        const auto state = machine.find_state(state_name);
        if (!state) {
            continue;
        }

        for (const auto& candidate : state->transitions_for(input)) {
            if (candidate) {
                next.insert(candidate->name);
            }
        }
    }

    return next;
}

}  // namespace

dfa transform(const nfa& machine, logger* log) {
    if (!machine.start_state()) {
        throw std::logic_error("nfa start state is not set");
    }

    if (log) {
        log->info("starting nfa -> dfa transform");
    }

    dfa result;
    for (char input : machine.input_signals()) {
        result.add_input(input);
    }

    std::map<std::set<std::string>, std::string> discovered;
    std::vector<std::set<std::string>> pending;

    const std::set<std::string> start_mixed = {machine.start_state()->name};
    const std::string start_name = mixed_state_name(start_mixed);

    discovered.emplace(start_mixed, start_name);
    result.add_state(start_name, mixed_state_is_final(start_mixed, machine));
    result.set_start_state(start_name);
    pending.push_back(start_mixed);
    if (log) {
        log->debug("discovered start mixed state '" + start_name + "'");
    }

    for (std::size_t index = 0; index < pending.size(); ++index) {
        const auto current = pending[index];
        const std::string current_name = discovered.at(current);
        if (log) {
            log->debug("expanding mixed state '" + current_name + "'");
        }

        for (char input : machine.input_signals()) {
            const auto next = next_mixed_state(current, input, machine);
            if (next.empty()) {
                if (log) {
                    log->debug(
                        "no transition from '" + current_name + "' on '" + std::string(1, input) + "'");
                }
                continue;
            }

            auto it = discovered.find(next);
            if (it == discovered.end()) {
                const std::string next_name = mixed_state_name(next);
                discovered.emplace(next, next_name);
                result.add_state(next_name, mixed_state_is_final(next, machine));
                pending.push_back(next);
                it = discovered.find(next);
                if (log) {
                    log->debug("discovered mixed state '" + next_name + "'");
                }
            }

            result.set_transition(current_name, input, it->second);
            if (log) {
                log->debug(
                    "dfa transition " + current_name + " --" + std::string(1, input) + "--> " +
                    it->second);
            }
        }
    }

    if (log) {
        log->info("finished nfa -> dfa transform");
    }
    return result;
}

bool validate_string(const std::string& input, const dfa& machine, logger* log) {
    if (log) {
        log->info("validating input string '" + input + "'");
    }
    return machine.accepts(input, log);
}

}  // namespace stm

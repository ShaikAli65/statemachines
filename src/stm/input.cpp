#include "input.hpp"

#include <sstream>
#include <stdexcept>

namespace stm {
namespace {

std::vector<std::string> split_tokens(const std::string& line) {
    std::istringstream stream(line);
    std::vector<std::string> tokens;
    std::string token;
    while (stream >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

std::vector<std::string> parse_named_tokens(
    const std::string& line,
    const std::string& expected_prefix) {
    if (line.rfind(expected_prefix, 0) != 0) {
        throw std::invalid_argument("expected line to start with '" + expected_prefix + "'");
    }

    return split_tokens(line.substr(expected_prefix.size()));
}

std::string require_line(std::istream& input, const char* description) {
    std::string line;
    if (!std::getline(input, line)) {
        throw std::invalid_argument(std::string("missing ") + description);
    }
    return line;
}

std::vector<char> parse_inputs(const std::vector<std::string>& tokens) {
    std::vector<char> inputs;
    for (const auto& token : tokens) {
        if (token.size() != 1) {
            throw std::invalid_argument("input symbols must be single characters");
        }
        inputs.push_back(token.front());
    }
    return inputs;
}

}  // namespace

nfa_spec parse_nfa(std::istream& input) {
    nfa_spec spec;

    spec.states = parse_named_tokens(require_line(input, "states line"), "states:");
    spec.inputs = parse_inputs(parse_named_tokens(require_line(input, "inputs line"), "inputs:"));

    const auto start = parse_named_tokens(require_line(input, "start line"), "start:");
    if (start.size() != 1) {
        throw std::invalid_argument("start line must define exactly one state");
    }
    spec.start_state = start.front();

    spec.final_states = parse_named_tokens(require_line(input, "finals line"), "finals:");

    const std::string transitions_header = require_line(input, "transitions header");
    if (transitions_header != "transitions:") {
        throw std::invalid_argument("expected 'transitions:' header");
    }

    for (std::string line; std::getline(input, line);) {
        if (line.empty()) {
            continue;
        }
        if (line == "end") {
            break;
        }

        const auto tokens = split_tokens(line);
        if (tokens.size() < 3 || tokens[1].size() != 1) {
            throw std::invalid_argument("invalid nfa transition line: " + line);
        }

        nfa_transition_spec transition;
        transition.from = tokens[0];
        transition.input = tokens[1].front();
        transition.to.assign(tokens.begin() + 2, tokens.end());
        spec.transitions.push_back(transition);
    }

    return spec;
}

dfa_spec parse_dfa(std::istream& input) {
    dfa_spec spec;

    spec.states = parse_named_tokens(require_line(input, "states line"), "states:");
    spec.inputs = parse_inputs(parse_named_tokens(require_line(input, "inputs line"), "inputs:"));

    const auto start = parse_named_tokens(require_line(input, "start line"), "start:");
    if (start.size() != 1) {
        throw std::invalid_argument("start line must define exactly one state");
    }
    spec.start_state = start.front();

    spec.final_states = parse_named_tokens(require_line(input, "finals line"), "finals:");

    const std::string transitions_header = require_line(input, "transitions header");
    if (transitions_header != "transitions:") {
        throw std::invalid_argument("expected 'transitions:' header");
    }

    for (std::string line; std::getline(input, line);) {
        if (line.empty()) {
            continue;
        }
        if (line == "end") {
            break;
        }

        const auto tokens = split_tokens(line);
        if (tokens.size() != 3 || tokens[1].size() != 1) {
            throw std::invalid_argument("invalid dfa transition line: " + line);
        }

        dfa_transition_spec transition;
        transition.from = tokens[0];
        transition.input = tokens[1].front();
        transition.to = tokens[2];
        spec.transitions.push_back(transition);
    }

    return spec;
}

nfa build_nfa(const nfa_spec& spec, logger* log) {
    nfa machine;

    if (log) {
        log->info("building nfa from parsed specification");
    }

    for (char input : spec.inputs) {
        machine.add_input(input);
    }

    for (const auto& state_name : spec.states) {
        machine.add_state(state_name);
        if (log) {
            log->debug("nfa state added: " + state_name);
        }
    }

    machine.set_start_state(spec.start_state);
    if (log) {
        log->info("nfa start state set to '" + spec.start_state + "'");
    }

    for (const auto& state_name : spec.final_states) {
        machine.add_state(state_name, true);
        if (log) {
            log->debug("nfa final state marked: " + state_name);
        }
    }

    for (const auto& transition : spec.transitions) {
        for (const auto& destination : transition.to) {
            machine.add_transition(transition.from, transition.input, destination);
            if (log) {
                log->debug(
                    "nfa transition " + transition.from + " --" + std::string(1, transition.input) +
                    "--> " + destination);
            }
        }
    }

    return machine;
}

dfa build_dfa(const dfa_spec& spec, logger* log) {
    dfa machine;

    if (log) {
        log->info("building dfa from parsed specification");
    }

    for (char input : spec.inputs) {
        machine.add_input(input);
    }

    for (const auto& state_name : spec.states) {
        machine.add_state(state_name);
        if (log) {
            log->debug("dfa state added: " + state_name);
        }
    }

    machine.set_start_state(spec.start_state);
    if (log) {
        log->info("dfa start state set to '" + spec.start_state + "'");
    }

    for (const auto& state_name : spec.final_states) {
        machine.add_state(state_name, true);
        if (log) {
            log->debug("dfa final state marked: " + state_name);
        }
    }

    for (const auto& transition : spec.transitions) {
        machine.set_transition(transition.from, transition.input, transition.to);
        if (log) {
            log->debug(
                "dfa transition " + transition.from + " --" + std::string(1, transition.input) +
                "--> " + transition.to);
        }
    }

    return machine;
}

}  // namespace stm

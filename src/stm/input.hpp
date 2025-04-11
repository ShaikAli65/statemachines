#pragma once

#include "dfa.hpp"
#include "logger.hpp"
#include "nfa.hpp"

#include <istream>
#include <string>
#include <vector>

namespace stm {

struct nfa_transition_spec {
    std::string from;
    char input = '\0';
    std::vector<std::string> to;
};

struct dfa_transition_spec {
    std::string from;
    char input = '\0';
    std::string to;
};

struct nfa_spec {
    std::vector<std::string> states;
    std::vector<char> inputs;
    std::string start_state;
    std::vector<std::string> final_states;
    std::vector<nfa_transition_spec> transitions;
};

struct dfa_spec {
    std::vector<std::string> states;
    std::vector<char> inputs;
    std::string start_state;
    std::vector<std::string> final_states;
    std::vector<dfa_transition_spec> transitions;
};

nfa_spec parse_nfa(std::istream& input);
dfa_spec parse_dfa(std::istream& input);

nfa build_nfa(const nfa_spec& spec, logger* log = nullptr);
dfa build_dfa(const dfa_spec& spec, logger* log = nullptr);

}  // namespace stm

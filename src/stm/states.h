#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace stm {

struct nfa_state {
    explicit nfa_state(std::string state_name = "_");

    void add_transition(char input, const std::shared_ptr<nfa_state>& next);
    std::vector<std::shared_ptr<nfa_state>> transitions_for(char input) const;

    bool is_final = false;
    std::string name;
    std::unordered_map<char, std::vector<std::shared_ptr<nfa_state>>> transitions;
};

struct dfa_state {
    explicit dfa_state(std::string state_name = "_");

    void set_transition(char input, const std::shared_ptr<dfa_state>& next);
    std::shared_ptr<dfa_state> transition_for(char input) const;

    bool is_final = false;
    std::string name;
    std::unordered_map<char, std::shared_ptr<dfa_state>> transitions;
};

}  // namespace stm

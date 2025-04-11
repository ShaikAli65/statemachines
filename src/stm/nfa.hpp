#pragma once

#include "states.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace stm {

class nfa {
public:
    nfa() = default;

    std::shared_ptr<nfa_state> add_state(const std::string& name, bool is_final = false);
    void set_start_state(const std::string& name);
    void add_input(char input);
    void add_transition(const std::string& from, char input, const std::string& to);

    bool has_state(const std::string& name) const;
    std::shared_ptr<nfa_state> find_state(const std::string& name) const;

    const std::shared_ptr<nfa_state>& start_state() const;
    const std::unordered_map<std::string, std::shared_ptr<nfa_state>>& states() const;
    const std::vector<std::shared_ptr<nfa_state>>& final_states() const;
    const std::vector<char>& input_signals() const;

private:
    std::shared_ptr<nfa_state> ensure_state(const std::string& name);
    bool has_input(char input) const;

    std::unordered_map<std::string, std::shared_ptr<nfa_state>> states_;
    std::vector<std::shared_ptr<nfa_state>> final_states_;
    std::vector<char> input_signals_;
    std::shared_ptr<nfa_state> start_state_;
};

}  // namespace stm

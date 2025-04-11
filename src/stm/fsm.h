#pragma once

#include "dfa.hpp"
#include "input.hpp"
#include "linear_bound.hpp"
#include "logger.hpp"
#include "nfa.hpp"
#include "push_down.hpp"
#include "states.h"
#include "turing.hpp"

#include <string>

namespace stm {

dfa transform(const nfa& machine, logger* log = nullptr);
bool validate_string(const std::string& input, const dfa& machine, logger* log = nullptr);

}  // namespace stm

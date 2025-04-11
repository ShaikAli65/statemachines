#include "stm/fsm.h"

#include <iostream>
#include <sstream>

int main() {
    std::istringstream input(R"(states: A B C
inputs: 0 1
start: A
finals: C
transitions:
A 0 A
A 1 A B
B 1 C
end
)");

    stm::logger log(std::cout, stm::log_level::debug);
    const stm::nfa_spec spec = stm::parse_nfa(input);
    const stm::nfa machine = stm::build_nfa(spec, &log);
    const stm::dfa converted = stm::transform(machine, &log);

    const char* accepted =
        stm::validate_string("11", converted, &log) ? "Accepted" : "Rejected";
    std::cout << accepted << '\n';
    return 0;
}

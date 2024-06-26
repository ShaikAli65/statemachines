//
// Created by 7862s on 12-02-2024.
//

#ifndef STATEMACHINES_SRC_STM_CPP
#define STATEMACHINES_SRC_STM_CPP
#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <vector>
#include <limits>
#include <sstream>
#include <set>
#include <stdexcept>
#include <cstring>
#include "stm.h"
#include "states.h"
#define LINE(len) for(int i = 0; i < len; i++ ) std::cout<<"-------";

namespace stm {
    class automata {
    public:
        virtual void print() const = 0;
        virtual void get_transition() = 0;
        virtual void get_prerequsites() = 0;
        virtual bool operator==(const automata &cmp) const = 0;
//        virtual void add_state(state _in) = 0;
//    protected:
        int n_inputs;
        int n_states;
    };
}
namespace stm::nfa {
    class stateMachine : public stm::automata{
    private:
        int n_states{}, n_inputs{};
        std::shared_ptr<state> null = std::make_shared<state>();
        std::unordered_map<std::string, std::shared_ptr<state>> _validstates{};
        std::vector<std::shared_ptr<state>> _finalstates{};

    public:
        std::shared_ptr<state> start_state;
        std::vector<char> _inputsignals;
        stateMachine() : n_inputs(0), n_states(0) {}
        void get_prerequsites() {
            std::cout<<"Enter number of valid states :",std::cin >> std::skipws >> n_states,std::cout << std::endl;
            std::string input;
            std::cout << "Enter starting state name: ",std::cin >> std::skipws >> input,std::cout << std::endl;
            start_state = std::make_shared<state>(input);
            _validstates[input] = start_state;
            for(size_t i = 1 ; i < n_states ; i++ )
            {
                std::string _in;
                std::cout << "Enter state (" << i + 1 << ") name: ",std::cin >> std::skipws >> _in, std::cout << std::endl;
                _validstates[_in] = std::make_shared<state>(_in);
            }
            std::cout<<"Enter number of inputs :",std::cin >> std::skipws >> n_inputs, std::cout << std::endl;
            _inputsignals.reserve(n_inputs);
            for(size_t i = 0 ; i < n_inputs ; i++ )
            {
                char _in;
                std::cout << "Enter input(" << i + 1 << "): ",std::cin >> std::skipws >>_in, std::cout << std::endl;
                _inputsignals.emplace_back(_in);
                null -> setoutput(_in,null);
            }
//            _validstates["_"] = nullptr;
            int n;
            std::cin.ignore();
            std::cout<<"\nEnter no.of Final States: ",std::cin >> n, std::cout << std::endl;
            while(n --> 0)
            {
                std::string r;
                std::cout<<"Enter finalstate(" << n << "): ",std::cin >> std::skipws >> r;
                _validstates[r] -> isfinal = true;
                _finalstates.emplace_back(_validstates[r]);
            }
            std::cout<<std::endl;
        }
        void get_transition(){
            std::cout << "\nEnter transition details of non-deterministic finite automata\n";
            std::cout << "(Fill using spaces for multiple outputs and press enter when you are finished):     \n\n";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            for (auto &[statename,state] : _validstates)
            {
                if (statename == "_")
                    continue;
                LINE(5) std::cout << "\n\n";
                std::cout << "For state :" << statename <<" "<< &(state -> outputStates) << "\n\n";
                for (const auto &sig : _inputsignals)
                {
                    std::vector<std::string> inputs;
                    char inp[400];
                    std::cout << "Input signal " << sig << ": ";
                    std::cin >> std::skipws;
                    std::cin.getline(inp,400,'\n');
                    std::string input(inp);
                    for(auto &a : stm::tokenize(input, ' '))
                    {
                        if (a == "_")
                            continue;
                        state -> setoutput(sig, _validstates[a]);
                    }
                    std::cout << std::endl;
                }
            }
            LINE(5) std::cout << "\n\n";

        }
        void add_state(std::shared_ptr<state> &_in) {
            _validstates[_in -> name] = _in;
        }
        void add_as_final_state(const std::shared_ptr<state> &_in) {
            _in -> isfinal = true;
            _finalstates.emplace_back(_in);
            _validstates[_in -> name] = _in;
        }
        const bool isfinal(const std::string &name) const {
            return _validstates.at(name) -> isfinal;
        }
        auto find(const std::string &name) const {
            return _validstates.find(name);
        }
        const auto begin() const {
            return _validstates.begin();
        }
        const auto end() const {
            return _validstates.end();
        }
        bool operator==(const stm::automata &cmp) const {
            return n_inputs == cmp.n_inputs && n_states == cmp.n_states;
        }
        void print() const{
            for(const auto &[k,v]:_validstates)
            {
                v -> print();
            }
        }
    // private:
    };
}
namespace stm::dfa {
    class stateMachine : public stm::automata {
        std::shared_ptr<state> null = std::make_shared<state>();
        std::unordered_map<std::string, std::shared_ptr<state>> _validstates{};
        std::vector<std::shared_ptr<state>> _finalstates{};
    public:
        int n_states{}, n_inputs{};
        std::shared_ptr<state> start_state{};
        std::vector<char> _inputsignals;
        void get_prerequsites() {
                std::cout << "Enter valid number of states :", std::cin >> n_states, std::cout << std::endl;
                std::string starting_state_name;
                std::cout << "Enter starting state name: ", std::cin >> std::skipws >> starting_state_name, std::cout << std::endl;
                start_state = std::make_shared<state>(starting_state_name);
                _validstates[starting_state_name] = start_state;
//                std::cout << "address of start state : " << &(start_state->outputStates) <<" "<< &(_validstates[starting_state_name]->outputStates) <<std::endl;
//                start_state = _validstates[starting_state_name];
                for (size_t i = 1; i < n_states; i++) {
                    std::string _in;
                    std::cout << "Enter state (" << i + 1 << ") name: ", std::cin >> std::skipws >> _in, std::cout << std::endl;
//                    state st(_in);
                    _validstates[_in] = std::make_shared<state>(_in);
//                    _validstates[_in] = st;
                }
                std::cout << "Enter number of inputs :", std::cin >> n_inputs, std::cout << std::endl;
                _inputsignals.reserve(n_inputs);
                for (size_t i = 0; i < n_inputs; i++) {
                    char _in;
                    std::cout << "Enter input(" << i + 1 << "): ", std::cin >> std::skipws >> _in, std::cout << std::endl;
                    _inputsignals.emplace_back(_in);
                    null -> setoutput(_in, null);
                }
//                _validstates["_"] = nullptr;
                int n;
                std::cin.ignore();
                std::cout << "\nEnter no.of Final States: ", std::cin >> n, std::cout << std::endl;
                while (n-- > 0) {
                    std::string r;
                    std::cout << "Enter finalstate(" << n << "): ", std::cin >> std::skipws >> r;
                    _validstates[r] -> isfinal = true;
                    _finalstates.emplace_back(_validstates[r]);
                }
                std::cout << std::endl;
        };
        void get_transition() {
            std::cout << "\nSTATES" << std::setw(3);
            for (const auto &sig: _inputsignals) {
                std::cout << "| " << std::setw(3) << sig << std::setw(5);
            }
            std::cout << "|\n---------";
            LINE(n_inputs)
            std::cout << std::endl;
            for (int i = 0; i < n_states; i++) {
                std::string key;
                std::cin >> std::skipws >> key;
                auto &key_state = _validstates[key];
                for (const auto &sig: _inputsignals) {
                    std::string r;
                    std::cin >> std::skipws >> r;
                    if (r == "_") continue;
                    key_state -> setoutput(sig, _validstates[r]);
                }
            }
            std::cout << std::endl;
        }
        void add_state(const std::shared_ptr<state> &_in) {
            _validstates[_in -> name] = _in;
            if (_in -> isfinal)
                _finalstates.emplace_back(_in);
        }
        void add_as_final_state(const std::shared_ptr<state> &_in) {
            _in -> isfinal = true;
            _finalstates.emplace_back(_in);
            _validstates[_in -> name] = _in;
        }
        auto find(const std::string &name) const {
            return _validstates.find(name);
        }
        auto begin() const {
            return _validstates.begin();
        }
        auto end() const {
            return _validstates.end();
        }
        const auto no_of_states() const {
            return _validstates.size();
        }
        bool operator==(const stm::automata &cmp)const {
            return n_inputs == cmp.n_inputs && n_states == cmp.n_states;
        }
        void print() const {
            int big_name_length = 0;
            for (const auto& [k, v] : _validstates) {
                if (k.size() > big_name_length)
                    big_name_length = k.size();
            }
            LINE(7)
            std::cout << "\n\nStarting state : " << start_state->name << "\n\n"; //  << &(start_state -> outputStates)
            LINE(7)
            std::cout << "\n\nFinal States : ";
            for (const auto& fs : _finalstates) {
                std::cout << fs->name << " ";
            }
            std::cout << "\n\n";
            LINE(7)
            std::cout << "\n\n";
            std::cout << std::setw(big_name_length + 3) << "STATES" << std::setw(4);
            for (const auto& sig : _inputsignals) {
                std::cout << "|" << std::setw(big_name_length) << sig << std::setw(5);
            }
            std::cout << "    |\n-----";
            LINE(n_inputs*big_name_length + 1)
            std::cout << std::endl;
            for (const auto& [k, v] : _validstates) {
                std::cout << "   " << std::left << std::setw(big_name_length + 3) << k;
                for (const auto& sig : _inputsignals) {
                    const auto& name = v->where(sig)->name;
                    std::cout << "| " << std::setw(big_name_length + 3) << name;
                }
                std::cout << "|\n";
            }
            std::cout << "-----";
            LINE(n_inputs*big_name_length + 1)
            std::cout << std::endl;
        }


        // private:
        stateMachine() : n_inputs(0), n_states(0){

        }
    };
}

#endif //STATEMACHINES_SRC_STM_CPP
/*
test cases:
// --- dfa
2
a
b
2
0
1
1
b
a
a
b
b
b
_
 //---- nfa
4
A
B
C
D
2
0
1
1
D
C
_
D
D
_
D
A
A B
  */
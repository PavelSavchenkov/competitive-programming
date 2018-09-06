#include <iostream>

#include "prewritten-code/algorithms/SuffixAutomaton.hpp"

void SuffixAutomaton_test() {
    SuffixAutomaton<100, 'A', 'Z'> aut;

    aut.build("ABACABA");

    aut.output(std::cerr);

    aut.get_node(0);

}

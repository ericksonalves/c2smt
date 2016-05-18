#include <iostream>

#include "options_parser.h"
#include "solver_runner.h"

int main(int argc, const char** argv)
{
    options_parser p(argc, argv);
    solver_runner z3_runner("z3", "/home/ericksonalves/Solvers/z3/bin/");
    std::cout << z3_runner.solver_command() << std::endl;
    std::cout << z3_runner.solver_path() << std::endl;
    return p.parse();
}

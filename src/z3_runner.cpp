#include "z3_runner.h"

z3_runner::z3_runner(const std::string& z3_path)
: solver_runner(z3_default_command, z3_path)
{
}

z3_runner::~z3_runner()
{
}

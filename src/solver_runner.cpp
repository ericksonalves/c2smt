#include "solver_runner.h"

solver_runner::solver_runner(const std::string& solver_command,
                             const std::string& solver_path)
: m_solver_command(solver_command), m_solver_path(solver_path)
{
}

solver_runner::~solver_runner()
{
}

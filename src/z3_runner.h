#ifndef Z3_RUNNER_H
#define Z3_RUNNER_H

#include <string>

#include "solver_runner.h"

const std::string z3_default_command = "z3 2>/dev/null";
const std::string z3_smt2_argument = "-smt2";
const std::string z3_version_argument = "-version";

class z3_runner : public solver_runner
{
public:
    z3_runner(const std::string& z3_path);
    ~z3_runner();
    inline const std::string run_file_command(const std::string& file_name) const
    {
        return m_solver_path + m_solver_command + " " + z3_smt2_argument + " " + file_name;
    }
    inline const std::string version_command() const
    {
        return m_solver_path + m_solver_command + " " + z3_version_argument;
    }
};

#endif

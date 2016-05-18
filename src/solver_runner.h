#ifndef SOLVER_RUNNER_H
#define SOLVER_RUNNER_H

#include <string>

class solver_runner
{
public:
    solver_runner(const std::string& solver_command,
                  const std::string& solver_path);
    virtual ~solver_runner();
    const std::string& solver_command() const
    {
        return m_solver_command;
    }
    const std::string& solver_path() const
    {
        return m_solver_path;
    }
protected:
    const std::string m_solver_command;
    const std::string m_solver_path;
};

#endif

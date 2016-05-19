#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "options_parser.h"
#include "z3_runner.h"

const unsigned int buffer_size = 512;

std::vector<std::string> execute_command(const std::string& command)
{
  const std::shared_ptr<FILE> pipe(popen(command.c_str(), "r"), pclose);
  std::vector<std::string> result;

  if (!pipe)
  {
    result.push_back("Opening pipe failed");
  }
  else
  {
    char buffer[buffer_size];
    while (!feof(pipe.get()))
    {
      if (fgets(buffer, buffer_size, pipe.get()) != nullptr)
      {
        result.push_back(buffer);
      }
    }
  }

  return result;
}

int main(int argc, const char** argv)
{
    const options_parser options_parser(argc, argv);
    const z3_runner z3_runner("/home/ericksonalves/Solvers/z3/bin/");

    std::cout << z3_runner.solver_command() << std::endl;
    std::cout << z3_runner.solver_path() << std::endl;

    const std::vector<std::string> output = execute_command(z3_runner.version_command());

    for (const std::string& line : output)
    {
        std::cout << line << std::endl;
    }

    return options_parser.parse();
}

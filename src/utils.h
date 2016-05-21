#ifndef UTILS_H
#define UTILS_H

#include <cstdio>
#include <memory>
#include <string>
#include <vector>

#include "z3_runner.h"

const unsigned int buffer_size = 512;
const std::string default_mode = "r";

namespace utils
{
    std::vector<std::string> execute_command(const std::string& command)
    {
      const std::shared_ptr<FILE> pipe(popen(command.c_str(), default_mode.c_str()), pclose);
      std::vector<std::string> result;

      if (pipe)
      {
        char buffer[buffer_size] = { 0 };
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

    inline bool check_z3_availability(const z3_runner& z3_runner)
    {
        const std::vector<std::string> output = utils::execute_command(z3_runner.version_command());

        return output.size() > 0;
    }
}

#endif

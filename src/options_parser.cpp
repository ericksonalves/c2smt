#include "options_parser.h"

#include <iostream>
#include <regex>

options_parser::options_parser(const int argc, const char** argv)
: m_argc(argc)
{
    int i;
    for (i = 1; i < m_argc; ++i)
    {
        m_argv.push_back(argv[i]);
    }
}

options_parser::~options_parser()
{
    m_argv.clear();
}

const int options_parser::parse() const
{
    int k = 0;
    const std::string regex_pattern = "(ericks)(.*)(n)";
    for (const std::string& option : m_argv)
    {
        if (std::regex_match(option, std::regex(regex_pattern)))
        {
            std::cout << option << " matches " << regex_pattern << std::endl;
        }
        else
        {
            k = k + 1;
        }
    }
    return 0;
}

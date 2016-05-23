#include "options_parser.h"

#include <fstream>
#include <iostream>
#include <regex>

#include "parser.h"
#include "preprocessor.h"
#include "utils.h"
#include "z3_runner.h"

options_parser::options_parser(const int argc, const char** argv)
    : m_argc(argc),
    m_is_help_requested(false),
    m_is_delete_file_set(false),
    m_is_input_file_set(false),
    m_is_version_requested(false),
    m_is_z3_path_set(false),
    m_input_file(""),
    m_z3_path("")
{
    unsigned int i;
    for (i = 1; i < m_argc; ++i)
    {
        m_argv.push_back(argv[i]);
    }
}

options_parser::~options_parser()
{
    m_argv.clear();
}

inline void options_parser::m_show_c2smt_header()
{
    std::cout << "* * *     C2SMT v0.0.1     * * *" << std::endl;
}

inline void options_parser::m_show_help()
{
    std::cout << "Usage:                         Purpose:" << std::endl;
    std::cout << " c2smt --help                   show help" << std::endl;
    std::cout << " c2smt --version                show version" << std::endl;
    std::cout << " c2smt file.c                   convert c code to smt-lib code" << std::endl;
}

const int options_parser::parse()
{
    bool can_continue = true;

    m_show_c2smt_header();

    for (const std::string& option : m_argv)
    {
        if (std::regex_match(option, std::regex(help_regex)))
        {
            m_is_help_requested = true;
        }
        else if (std::regex_match(option, std::regex(version_regex)))
        {
            m_is_version_requested = true;
        }
        else if (std::regex_match(option, std::regex(input_file_regex)))
        {
            m_input_file = option;
            m_is_input_file_set = true;
        }
        else if (std::regex_match(option, std::regex(delete_file_regex)))
        {
            m_is_delete_file_set = true;
        }
        else if (std::regex_match(option, std::regex(z3_path_regex)))
        {
            m_z3_path = option.substr(option.find("=") + 1);
            m_is_z3_path_set = true;
        }
        else
        {
            std::cout << "Unknown argument: " << option << std::endl;
            can_continue = false;
        }
    }

    if (!can_continue)
    {
        return error_unknown_option;
    }

    if (m_is_version_requested)
    {
        return 0;
    }

    if (m_is_help_requested)
    {
        m_show_help();
        return 0;
    }

    if (!m_is_input_file_set)
    {
        return error_no_input_file;
    }
    else
    {
        std::cout << "Checking file availability in " << m_input_file << std::endl;
        std::fstream file;
        file.open(m_input_file);
        if (!file.is_open())
        {
            return error_file_unavailable;
        }
    }

    std::cout << "Checking Z3 availability in ";
    std::cout << (m_is_z3_path_set ? m_z3_path : "$PATH") << std::endl;

    const z3_runner z3_runner(m_z3_path);

    const bool is_z3_available = utils::check_z3_availability(z3_runner);

    if (!is_z3_available)
    {
        return error_z3_unavailable;
    }

    preprocessor preprocessor(m_input_file);
    std::vector<std::string> code = preprocessor.process();

    // Comment this line to result in an unsat code
    code.erase(code.end() - 1);

    std::cout << "Generating SMT-LIB code" << std::endl;

    parser parser;
    parser.parse(code);

    std::vector<std::string> output = utils::execute_command(z3_runner.run_file_command("c2smt.smt2"));

    size_t end = output[0].size() - 1;

    std::cout << "Generated code is ";

    for (size_t i = 0; i != end; ++i)
    {
        std::cout << output[0][i];
    }

    std::cout << std::endl;

    if (m_is_delete_file_set)
    {
        utils::execute_command("rm -rf c2smt.smt2");
    }

    return 0;
}

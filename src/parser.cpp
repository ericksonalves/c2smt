#include "parser.h"

#include <fstream>
#include <iostream>
#include <regex>

#include "ast.h"

parser::parser()
{
}

parser::~parser()
{
}

inline std::string parser::m_declare_assert_expr(const std::string& expr)
{
    return "(assert " + expr + ")";
}

inline std::string parser::m_declare_const_expr(const std::string& var,
                                                  const std::string& type)
{
    return "(declare-const " + var + " " + type + ")";
}

inline std::string parser::m_to_type(const std::string& raw_type)
{
    if (raw_type.compare("int") == 0)
    {
        return "Int";
    }
    else if (raw_type.compare("float") == 0)
    {
        return "Real";
    }
    return "";
}

void parser::parse(const std::vector<std::string>& code)
{
    std::ofstream smt_lib_file;
    smt_lib_file.open(smt2_file_name);
    for (const std::string& statement : code)
    {
        const size_t limiter_position = statement.find(':');

        const std::string left_side = statement.substr(0, limiter_position);
        const std::string right_side = statement.substr(limiter_position + 1);
        const size_t assignment_position = right_side.find('=');
        const std::string var = right_side.substr(0, assignment_position - 1);

        smt_lib_file << m_declare_const_expr(var, m_to_type(left_side)) << std::endl;

        ast node(right_side);
        smt_lib_file << m_declare_assert_expr(node.to_smt_lib_expr()) << std::endl;

        if (std::regex_match(var, std::regex(n_regex)))
        {
            smt_lib_file << m_declare_assert_expr("(> " + var + " 0)") << std::endl;
        }
    }

    smt_lib_file << "(check-sat)" << std::endl;
}

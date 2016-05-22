#include "preprocessor.h"

#include <iostream>
#include <fstream>
#include <regex>
#include <sstream>
#include <unordered_map>

preprocessor::preprocessor(const std::string& file_path)
: m_file_path(file_path)
{
}

preprocessor::~preprocessor()
{
}

const size_t preprocessor::m_find_math_operand(const std::string& expression)
{
    const size_t division_operand_position = expression.find(division_operand);
    if (division_operand_position != std::string::npos)
    {
        return division_operand_position;
    }

    const size_t minus_operand_position = expression.find(minus_operand);
    if (minus_operand_position != std::string::npos)
    {
        return minus_operand_position;
    }

    return std::string::npos;
}

size_t preprocessor::m_find_non_space_char(const std::string& str)
{
    for (size_t i = 0; i < str.size(); ++i)
    {
        if (str[i] != ' ')
        {
            return i;
        }
    }
    return 0;
}

unsigned int preprocessor::m_find_statement(const std::vector<std::string>& code,
                                            const std::string& regex,
                                            const unsigned int begin)
{
    unsigned int j = -1, end = code.size();
    for (unsigned int i = begin; i < end; ++i)
    {
        if (std::regex_match(code[i], std::regex(regex)))
        {
            j = i;
            break;
        }
    }
    return j;
}

unsigned int preprocessor::m_get_offset_unwind_steps(const std::string& expression)
{
    const size_t first_space = expression.find(' ') + 1;
    const size_t second_space = expression.find(' ', first_space + 1);
    const std::string operand = expression.substr(first_space, second_space - first_space);
    return (operand.compare(">") == 0 ? 0 : 1);
}

bool preprocessor::m_is_var_declaration(const std::string& statement)
{
    bool result = std::regex_match(statement, std::regex(int_no_space_declaration_regex));
    result = result || std::regex_match(statement, std::regex(float_no_space_declaration_regex));
    return result;
}

std::vector<std::string> preprocessor::process()
{
    std::vector<std::string>* code = new std::vector<std::string>;
    std::string line;
    std::ifstream infile(m_file_path);

    while (std::getline(infile, line))
    {
        std::istringstream iss(line);
        code->push_back(line);
    }

    unsigned int j = m_find_statement(*code, main_regex);
    unsigned int k = code->size() - 2;

    std::vector<std::string> code_to_parse;

    std::unordered_map<std::string, int> initial_values;

    for (unsigned int i = j + 1; i <= k - 1; ++i)
    {
        if (std::regex_match((*code)[i], std::regex(int_declaration_regex)))
        {
            const size_t begin = m_find_non_space_char((*code)[i]);
            const size_t type_end = (*code)[i].find(' ', begin);
            const size_t end = (*code)[i].find('=');
            const std::string declaration = (*code)[i].substr(begin, type_end - begin);
            const std::string var = (*code)[i].substr(type_end + 1, end - type_end - 2);
            const std::string assignment = (*code)[i].substr(end, (*code)[i].size() - 1);
            int initial_value = std::stoi(assignment.substr(2));
            initial_values.insert(std::make_pair(var, initial_value));
            code_to_parse.push_back(declaration + " " + var + ";");
            code_to_parse.push_back(var + " " + assignment);
        }
        else if (std::regex_match((*code)[i], std::regex(post_fixed_decrement_regex)))
        {
            const size_t begin = m_find_non_space_char((*code)[i]);
            const size_t end = (*code)[i].find('-');
            const std::string var = (*code)[i].substr(begin, end - begin);
            (*code)[i] = var + " = " + var + " - 1;";
            code_to_parse.push_back((*code)[i]);
        }
        else
        {
            code_to_parse.push_back((*code)[i]);
        }
    }

    delete code;

    unsigned int while_body_begin = m_find_statement(code_to_parse, while_regex) + 1;
    unsigned int while_body_end = m_find_statement(code_to_parse, closing_curly_braces_regex, while_body_begin) - 1;
    unsigned int while_condition_line = while_body_begin - 1;

    size_t condition_begin = code_to_parse[while_condition_line].find('(') + 1;
    size_t condition_end = code_to_parse[while_condition_line].find(')');
    std::string condition = code_to_parse[while_condition_line].substr(condition_begin, condition_end - condition_begin);

    unsigned int offset = m_get_offset_unwind_steps(condition);

    const size_t condition_first_space = condition.find(' ');
    const std::string condition_var = condition.substr(0, condition_first_space);

    unsigned int times = initial_values[condition_var] - offset;

    std::vector<std::string> unwound_code_to_parse;

    for (unsigned int i = 0; i < while_body_begin - 1; ++i)
    {
        std::string code_to_insert = code_to_parse[i].substr(m_find_non_space_char(code_to_parse[i]));
        unwound_code_to_parse.push_back(code_to_insert);
    }

    // Get this number from expr
    for (unsigned int i = 0; i < times; ++i)
    {
        for (unsigned int j = while_body_begin; j <= while_body_end; ++j)
        {
            std::string code_to_insert = code_to_parse[j].substr(m_find_non_space_char(code_to_parse[j]));
            unwound_code_to_parse.push_back(code_to_insert);
        }
    }

    for (unsigned int i = while_body_end + 2; i < code_to_parse.size(); ++i)
    {
        std::string code_to_insert = code_to_parse[i].substr(m_find_non_space_char(code_to_parse[i]));
        unwound_code_to_parse.push_back(code_to_insert);
    }

    std::unordered_map<std::string, unsigned int> vars;
    std::unordered_map<std::string, std::string> types;

    std::vector<std::string> v;

    for (unsigned int i = 0; i < unwound_code_to_parse.size(); ++i)
    {
        if (m_is_var_declaration(unwound_code_to_parse[i]))
        {
            const size_t begin = m_find_non_space_char(unwound_code_to_parse[i]);
            const size_t type_end = unwound_code_to_parse[i].find(' ', begin);
            const size_t end = unwound_code_to_parse[i].find(';');
            const std::string declaration = unwound_code_to_parse[i].substr(begin, type_end - begin);
            const std::string var = unwound_code_to_parse[i].substr(type_end + 1, end - 1 - type_end);
            vars.insert(std::make_pair(var, 0));
            types.insert(std::make_pair(var, declaration));
        }
        else if (std::regex_match(unwound_code_to_parse[i], std::regex(assignemnt_regex)))
        {
            const size_t assignment_operator = unwound_code_to_parse[i].find('=');

            const std::string var = unwound_code_to_parse[i].substr(0, assignment_operator - 1);
            std::unordered_map<std::string, std::string>::const_iterator assignee_type = types.find(var);
            const unsigned int assignee_counter = vars[var];
            std::string left_operation = unwound_code_to_parse[i].substr(assignment_operator);

            const size_t operand_position = m_find_math_operand(left_operation);

            if (operand_position != std::string::npos)
            {
                const std::string first_operand = left_operation.substr(2, operand_position - 3);
                const std::string second_operand = left_operation.substr(operand_position + 2, left_operation.size() - operand_position - 3);

                std::unordered_map<std::string, unsigned int>::const_iterator first_operand_find = vars.find(first_operand);
                std::unordered_map<std::string, unsigned int>::const_iterator second_operand_find = vars.find(second_operand);

                const std::string operand = left_operation.substr(operand_position, 1);

                std::string first = first_operand;
                if (first_operand_find != vars.end())
                {
                    first += std::to_string(first_operand_find->second - 1);
                }
                std::string second = second_operand;
                if (second_operand_find != vars.end())
                {
                    second += std::to_string(second_operand_find->second - 1);
                }
                left_operation = "= " + first + " " + operand + " " + second + ";";
            }

            unwound_code_to_parse[i] = var + std::to_string(assignee_counter) + " " + left_operation;
            vars[var] = assignee_counter + 1;

            v.push_back(assignee_type->second + ":" + unwound_code_to_parse[i]);
        }
    }

    return v;
}

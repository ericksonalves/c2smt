#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include <string>
#include <vector>

const std::string assignemnt_regex = "(.*)(=)(.*)";
const std::string closing_curly_braces_regex = "( .+)(})(.*)";
const std::string int_declaration_regex = "( .*)(int)(.*)";
const std::string int_no_space_declaration_regex = "(int)(.*)";
const std::string float_declaration_regex = "( .*)(float)(.*)";
const std::string float_no_space_declaration_regex = "(float)(.*)";
const std::string main_regex = "(int main)(.*)";
const std::string post_fixed_decrement_regex = "(.*)(--)(.*)";
const std::string while_regex = "( .+)(while)(.*)";

const char division_operand = '/';
const char minus_operand = '-';

class preprocessor
{
public:
    preprocessor(const std::string& file_path);
    ~preprocessor();
    std::vector<std::string> process();
private:
    const std::string m_file_path;

    const size_t m_find_math_operand(const std::string& expression);
    size_t m_find_non_space_char(const std::string& str);
    unsigned int m_find_statement(const std::vector<std::string>& code,
                                const std::string& regex,
                                const unsigned int begin = 0);
    unsigned int m_get_offset_unwind_steps(const std::string& expression);
    bool m_is_var_declaration(const std::string& statement);
};

#endif

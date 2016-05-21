#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>
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

unsigned int find_statement(const std::vector<std::string>& code, const std::string& regex, const unsigned int begin = 0)
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

size_t find_non_space_char(const std::string& str)
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

void unwind_while(const std::string& code)
{
}

bool is_var_declaration(const std::string& statement)
{
    bool result = std::regex_match(statement, std::regex(int_no_space_declaration_regex));
    result = result || std::regex_match(statement, std::regex(float_no_space_declaration_regex));
    return result;
}

int main(int argc, const char** argv)
{
    std::vector<std::string>* code = new std::vector<std::string>;

    code->push_back("#include <stdio.h>");
    code->push_back("int main(int argc, char** argv) {");
    code->push_back("    float c;");
    code->push_back("    int n = 4;");
    code->push_back("    while (n > 0) {");
    code->push_back("        c = 10 / n;");
    code->push_back("        n--;");
    code->push_back("    }");
    code->push_back("    return 0;");
    code->push_back("}");

    unsigned int j = find_statement(*code, main_regex);
    unsigned int k = code->size() - 2;

    std::vector<std::string> code_to_parse;

    for (unsigned int i = j + 1; i <= k - 1; ++i)
    {
        if (std::regex_match((*code)[i], std::regex(int_declaration_regex)))
        {
            const size_t begin = find_non_space_char((*code)[i]);
            const size_t type_end = (*code)[i].find(' ', begin);
            const size_t end = (*code)[i].find('=');
            const std::string declaration = (*code)[i].substr(begin, type_end - begin);
            const std::string var = (*code)[i].substr(type_end + 1, end - type_end - 2);
            const std::string assignment = (*code)[i].substr(end, (*code)[i].size() - 1);
            code_to_parse.push_back(declaration + " " + var + ";");
            code_to_parse.push_back(var + " " + assignment);
        }
        else if (std::regex_match((*code)[i], std::regex(post_fixed_decrement_regex)))
        {
            const size_t begin = find_non_space_char((*code)[i]);
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

    unsigned int while_body_begin = find_statement(code_to_parse, while_regex) + 1;
    unsigned int while_body_end = find_statement(code_to_parse, closing_curly_braces_regex, while_body_begin) - 1;
    unsigned int while_condition_line = while_body_begin - 1;

    size_t condition_begin = code_to_parse[while_condition_line].find('(') + 1;
    size_t condition_end = code_to_parse[while_condition_line].find(')');
    std::string condition = code_to_parse[while_condition_line].substr(condition_begin, condition_end - condition_begin);
    std::cout << condition << std::endl;

    std::vector<std::string> unwound_code_to_parse;

    for (unsigned int i = 0; i < while_body_begin - 1; ++i)
    {
        std::string code_to_insert = code_to_parse[i].substr(find_non_space_char(code_to_parse[i]));
        unwound_code_to_parse.push_back(code_to_insert);
    }

    for (unsigned int i = 0; i < 4; ++i)
    {
        for (unsigned int j = while_body_begin; j <= while_body_end; ++j)
        {
            std::string code_to_insert = code_to_parse[j].substr(find_non_space_char(code_to_parse[j]));
            unwound_code_to_parse.push_back(code_to_insert);
        }
    }

    for (unsigned int i = while_body_end + 2; i < code_to_parse.size(); ++i)
    {
        std::string code_to_insert = code_to_parse[i].substr(find_non_space_char(code_to_parse[i]));
        unwound_code_to_parse.push_back(code_to_insert);
    }

    // unwound_code_to_parse.erase(unwound_code_to_parse.end() - 1);

    std::cout << "Unwound code" << std::endl;

    std::unordered_map<std::string, unsigned int> vars;
    std::unordered_map<std::string, std::string> types;

    for (unsigned int i = 0; i < unwound_code_to_parse.size(); ++i)
    {
        std::cout << unwound_code_to_parse[i] << std::endl;
    }

    std::cout << "Test" << std::endl;

    for (unsigned int i = 0; i < unwound_code_to_parse.size(); ++i)
    {
        if (is_var_declaration(unwound_code_to_parse[i]))
        {
            const size_t begin = find_non_space_char(unwound_code_to_parse[i]);
            const size_t type_end = unwound_code_to_parse[i].find(' ', begin);
            const size_t end = unwound_code_to_parse[i].find(';');
            const std::string declaration = unwound_code_to_parse[i].substr(begin, type_end - begin);
            const std::string var = unwound_code_to_parse[i].substr(type_end + 1, end - 1 - type_end);
            vars.insert(std::make_pair(var, 0));
            types.insert(std::make_pair(var, declaration));
        }
        else if (std::regex_match(unwound_code_to_parse[i], std::regex(assignemnt_regex)))
        {
            size_t assignemnt_operator = unwound_code_to_parse[i].find('=');
            const std::string var = unwound_code_to_parse[i].substr(0, assignemnt_operator - 1);
            const unsigned int counter = vars[var];
            std::cout << "Assignment in " << types[var] << " var " << var << counter << " => ";
            vars[var] = counter + 1;
        }
        std::cout << unwound_code_to_parse[i] << std::endl;
    }

    return 0;
}

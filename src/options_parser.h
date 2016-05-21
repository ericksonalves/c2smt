#ifndef OPTIONS_PARSER_H
#define OPTIONS_PARSER_H

#include <string>
#include <vector>

const std::string help_regex = "(--help)";
const std::string input_file_regex = "(.*)(.c)";
const std::string version_regex = "(--version)";
const std::string z3_path_regex = "(--with-z3=)(.*)";

const unsigned int error_no_input_file = 1;
const unsigned int error_unknown_option = 2;
const unsigned int error_z3_unavailable = 3;

class options_parser
{
public:
    options_parser(const int argc, const char** argv);
    ~options_parser();
    const int parse();
private:
    const int m_argc;
    bool m_is_help_requested;
    bool m_is_input_file_set;
    bool m_is_version_requested;
    bool m_is_z3_path_set;
    std::string m_input_file;
    std::string m_z3_path;
    std::vector<std::string> m_argv;

    inline void m_error_no_input_file();
    inline void m_show_c2smt_header();
    inline void m_show_help();
};

#endif

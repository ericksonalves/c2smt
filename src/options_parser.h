#ifndef OPTIONS_PARSER_H
#define OPTIONS_PARSER_H

#include <string>
#include <vector>

class options_parser
{
public:
    options_parser(const int argc, const char** argv);
    ~options_parser();
    const int parse() const;
private:
    const int m_argc;
    std::vector<std::string> m_argv;
};

#endif

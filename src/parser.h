#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

const std::string n_regex = "(n)(.*)";
const std::string smt2_file_name = "c2smt.smt2";

class parser
{
public:
    parser();
    ~parser();
    void parse(const std::vector<std::string>& code);
private:
    inline std::string m_declare_assert_expr(const std::string& expr);
    inline std::string m_declare_const_expr(const std::string& var,
                                            const std::string& type);
    inline std::string m_to_type(const std::string& raw_type);
};

#endif

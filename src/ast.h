#ifndef AST_H
#define AST_H

#include <string>

class ast
{
public:
    ast(const std::string& statement);
    ~ast();
    std::string to_smt_lib_expr();
private:

    struct ast_node
    {
        std::string token;
        struct ast_node* left;
        struct ast_node* right;
    };

    typedef struct ast_node ast_node;

    ast_node* m_ast_node;

    void m_create_ast(const std::string& statement, ast_node** ast_node);
    void m_delete_ast(ast_node** ast_node);
    std::string m_print_ast(ast_node* ast_node);
};

#endif

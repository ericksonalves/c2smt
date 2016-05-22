#include "ast.h"

#include <iostream>

ast::ast(const std::string& statement)
: m_ast_node(nullptr)
{
    m_create_ast(statement, &m_ast_node);
}

ast::~ast()
{
    m_delete_ast(&m_ast_node);
}

void ast::m_create_ast(const std::string& statement, ast_node** ast_node)
{
    if (statement.compare("") == 0)
    {
        return;
    }

    const size_t assignment_operator = statement.find('=');
    const size_t division_operator = statement.find('/');
    const size_t minus_operator = statement.find('-');
    size_t statement_operator = std::string::npos;

    *ast_node = new ast::ast_node;

    if (assignment_operator != std::string::npos)
    {
        statement_operator = assignment_operator;
        (*ast_node)->token = "=";
    }
    else if (division_operator != std::string::npos)
    {
        statement_operator = division_operator;
        (*ast_node)->token = "/";
    }
    else if (minus_operator != std::string::npos)
    {
        statement_operator = minus_operator;
        (*ast_node)->token = "-";
    }

    if (statement_operator != std::string::npos)
    {
        const std::string left_side = statement.substr(0, statement_operator - 1);
        const std::string right_side = statement.substr(statement_operator + 2, statement.size() - statement_operator - 2);
        m_create_ast(left_side, &((*ast_node)->left));
        m_create_ast(right_side, &((*ast_node)->right));
    }
    else
    {
        (*ast_node)->token = statement;
        (*ast_node)->left = nullptr;
        (*ast_node)->right = nullptr;
    }
}

void ast::m_delete_ast(ast_node** ast_node)
{
    if (*ast_node != nullptr)
    {
        m_delete_ast(&(*ast_node)->left);
        m_delete_ast(&(*ast_node)->right);
        delete *ast_node;
    }
}

std::string ast::m_print_ast(ast_node* ast_node)
{
    std::string str("");

    if (ast_node != nullptr)
    {
        const bool has_left_child = ast_node->left != nullptr;
        const bool has_right_child = ast_node->right != nullptr;
        const bool has_child = has_left_child || has_right_child;
        str += (has_child ? "(" : "") + ast_node->token;
        if (has_left_child)
        {
            str += " ";
            str += m_print_ast(ast_node->left);
        }
        if (has_right_child)
        {
            str += " ";
            str += m_print_ast(ast_node->right);
        }
        str += (has_child ? ")" : "");
    }

    return str;
}

std::string ast::to_smt_lib_expr()
{
    return m_print_ast(m_ast_node);
}

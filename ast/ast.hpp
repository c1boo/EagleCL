#pragma once

#include <string>
#include <vector>
#include "token.hpp"

namespace ast
{
    // Base class for a node in AST
    class Node
    {
    public:
        virtual std::string tokenLiteral() const = 0;
        virtual ~Node() = default;
    };

    // A statement node in AST
    class Statement : public Node
    {
    public:
        virtual void statementNode() const = 0;
    };

    // An expression node in AST
    class Expression : public Node
    {
    public:
        virtual void expressionNode() const = 0;
    };

    // Program node is the root node of AST. Every statement produced is stored in this node.
    class Program : public Node
    {
    public:
        std::vector<Statement *> statements;

        std::string tokenLiteral() const override;
    };

    // Identifier node that holds the corresponding token and value of the identifier.
    class Identifier : public Expression
    {
    public:
        token::Token token;
        std::string_view value;

        void expressionNode() const override;
        std::string tokenLiteral() const override { return token.literal; };
    };

    // Var statement that has a tokentype of TokenType::Var and points to the identifier and expression in the statement.
    class VarStatement : public Statement
    {
    public:
        token::Token token;
        Identifier *name;
        Expression *value;

        void statementNode() const override;
        std::string tokenLiteral() const override { return token.literal; };
    };
}

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
        virtual std::string toString() const = 0;
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

        ~Program() override
        {
            for (auto stmt : statements)
            {
                delete stmt;
            }
        }

        std::string tokenLiteral() const override;

        // Writes the return value of each statements toString method to a buffer.
        // And then it returns the buffer as a string.
        // It is a function helpful for debugging
        std::string toString() const override;
    };

    // Identifier node that holds the corresponding token and value of the identifier.
    class Identifier : public Expression
    {
    public:
        token::Token token;
        std::string value;

        Identifier() = default;
        Identifier(token::Token tkn, std::string val) : token{tkn}, value{val}
        {
        }

        void expressionNode() const override {};
        std::string tokenLiteral() const override { return token.literal; };
        std::string toString() const override { return value; };
    };

    // Var statement that has a tokentype of TokenType::Var and points to the identifier and expression in the statement.
    class VarStatement : public Statement
    {
    public:
        token::Token token;
        Identifier *name;
        Expression *value;

        VarStatement() = default;
        VarStatement(token::Token tkn, Identifier *ident, Expression *expression)
            : token{tkn}, name{ident}, value{expression}
        {
        }

        ~VarStatement() override
        {
            delete name;
            delete value;
        }

        void statementNode() const override {};
        std::string tokenLiteral() const override { return token.literal; };

        // Returns the var statement as a string in a format of "identifier name = expression;"
        // Helpful function for debugging and comparing to other statements
        std::string toString() const override;
    };

    class ReturnStatement : public Statement
    {
    public:
        token::Token token;
        Expression *value;

        ReturnStatement() = default;
        ReturnStatement(token::Token tkn, Expression *expression)
            : token{token}, value{expression}
        {
        }

        ~ReturnStatement() override
        {
            delete value;
        }

        void statementNode() const override {};
        std::string tokenLiteral() const override { return token.literal; };

        // Returns the return statement in a format of "return expression;"
        // Helpful function for debugging and comparing to other statements
        std::string toString() const override;
    };

    class ExpressionStatement : public Statement
    {
    public:
        token::Token token;
        Expression *value;

        ExpressionStatement() = default;
        ExpressionStatement(token::Token tkn, Expression *expression)
            : token{token}, value{expression}
        {
        }

        ~ExpressionStatement() override
        {
            delete value;
        }

        void statementNode() const override {};
        std::string tokenLiteral() const override { return token.literal; };

        // Returns the expression as a string
        std::string toString() const override;
    };
}

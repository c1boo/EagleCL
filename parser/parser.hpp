#pragma once

#include "token.hpp"
#include "lexer.hpp"
#include "ast.hpp"

class Parser
{
public:
    lexer::Lexer *lexer;

    token::Token currentToken;
    token::Token peekToken;

    Parser(lexer::Lexer *lexer) : lexer(lexer)
    {
        nextToken();
        nextToken();
    }

    void nextToken();
    ast::Program *parseProgram();
};
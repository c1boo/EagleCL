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

    ~Parser()
    {
        delete lexer;
    }

    void nextToken();
    ast::Program *parseProgram();
    ast::Statement *parseStatement();
    ast::VarStatement *parseVarStatement();
    bool expectPeek(token::TokenType type);
    bool peekTokenIs(token::TokenType type);
    bool currentTokenIs(token::TokenType type);
};
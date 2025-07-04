#pragma once

#include "token.hpp"
#include "lexer.hpp"
#include "ast.hpp"

class Parser
{
private:
    std::vector<std::string> errors{};

    // Appends an error to errors stack if peek token is not the expected token
    void peekError(token::TokenType type);

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

    // checks the type of the peekToken and only if the type is correct does it
    // advance the tokens by calling nextToken
    bool expectPeek(token::TokenType type);

    // checks whether the peek token is the given type
    bool peekTokenIs(token::TokenType type);

    // checks whether the current token is the given type
    bool currentTokenIs(token::TokenType type);

    // Getter for happened errors while parsing
    std::vector<std::string> getErrors() { return errors; }
};
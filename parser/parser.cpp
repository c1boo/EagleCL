#include "parser.hpp"

void Parser::nextToken()
{
    currentToken = peekToken;
    peekToken = lexer->nextToken();
}

// Testing parser branch
// Testing testing
// Another testing
ast::Program *Parser::parseProgram()
{
    auto *program = new ast::Program();
    while (currentToken.type != token::EOF_)
    {
        ast::Statement *statement = parseStatement();
        if (statement != nullptr)
        {
            program->statements.push_back(statement);
        }

        nextToken();
    }

    return program;
}

ast::Statement *parseStatement()
{
}
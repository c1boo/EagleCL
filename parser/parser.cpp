#include "parser.hpp"
#include <iostream>

void Parser::nextToken()
{
    currentToken = peekToken;
    peekToken = lexer->nextToken();
}

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

ast::Statement *Parser::parseStatement()
{
    if (currentToken.type == token::VAR)
    {
        auto *vs = parseVarStatement();
        return vs;
    }
    else
    {
        return nullptr;
    }
}

ast::VarStatement *Parser::parseVarStatement()
{
    auto *statement = new ast::VarStatement();
    statement->token = currentToken;

    if (!expectPeek(token::IDENT))
    {
        return nullptr;
    }

    statement->name = new ast::Identifier();
    statement->name->token = currentToken;
    statement->name->value = currentToken.literal;

    if (!expectPeek(token::ASSIGN))
    {
        return nullptr;
    }

    // TODO: We are skipping expressions until we encounter a semiclon (FOR NOW!)
    while (!currentTokenIs(token::SEMICOLON))
    {
        nextToken();
    }

    return statement;
}

bool Parser::expectPeek(token::TokenType type)
{
    if (peekTokenIs(type))
    {
        nextToken();
        return true;
    }
    else
    {
        return false;
    }
}

bool Parser::peekTokenIs(token::TokenType type)
{
    return peekToken.type == type;
}

bool Parser::currentTokenIs(token::TokenType type)
{
    return currentToken.type == type;
}
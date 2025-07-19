#include "parser.hpp"
#include <iostream>
#include <sstream>

void Parser::peekError(token::TokenType type)
{
    std::ostringstream oss;
    oss << "Expected next token to be " << type << ", got " << peekToken.type << " instead.";
    errors.push_back(oss.str());
}

void Parser::noPrefixParseFnError(token::TokenType type)
{
    std::ostringstream oss;
    oss << "No prefix parse function found for token type " << type << ".";
    errors.push_back(oss.str());
}

Parser::Parser(lexer::Lexer *lexer) : lexer{lexer}
{
    nextToken();
    nextToken();

    registerPrefix(token::IDENT,
                   [this]
                   { return this->parseIdentifier(); });

    registerPrefix(token::INT,
                   [this]
                   { return this->parseIntegerLiteral(); });

    registerPrefix(token::BANG,
                   [this]
                   { return this->parsePrefixExpression(); });

    registerPrefix(token::MINUS,
                   [this]
                   { return this->parsePrefixExpression(); });
}

void Parser::nextToken()
{
    currentToken = peekToken;
    peekToken = lexer->nextToken();
}

ast::Program *Parser::parseProgram()
{
    auto *program = new ast::Program();
    while (!currentTokenIs(token::EOF_))
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
    if (currentTokenIs(token::VAR))
    {
        return parseVarStatement();
    }
    else if (currentTokenIs(token::RETURN))
    {
        return parseReturnStatement();
    }
    else
    {
        return parseExpressionStatement();
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

ast::ReturnStatement *Parser::parseReturnStatement()
{
    auto *statement = new ast::ReturnStatement();
    statement->token = currentToken;

    nextToken();

    // TODO: We are skipping expressions until we encounter a semiclon (FOR NOW!)
    while (!currentTokenIs(token::SEMICOLON))
    {
        nextToken();
    }

    return statement;
}

ast::ExpressionStatement *Parser::parseExpressionStatement()
{
    auto *statement = new ast::ExpressionStatement(currentToken, parseExpression(Precedence::LOWEST));

    if (peekTokenIs(token::SEMICOLON))
    {
        nextToken();
    }

    return statement;
}

ast::Expression *Parser::parseExpression(Precedence precedence)
{
    prefixParseFn prefix = prefixParseFunctions[currentToken.type];

    if (!prefix)
    {
        noPrefixParseFnError(currentToken.type);
        return nullptr;
    }

    ast::Expression *leftExpression = prefix();
    return leftExpression;
}

ast::Expression *Parser::parseIdentifier()
{
    return new ast::Identifier(currentToken, currentToken.literal);
}

ast::Expression *Parser::parseIntegerLiteral()
{
    int64_t val{};
    try
    {
        val = std::stoi(currentToken.literal);
    }
    catch (const std::exception &e)
    {
        std::ostringstream oss;
        oss << "Could not parse literal " << currentToken.literal << " to INT literal.";
        errors.push_back(oss.str());

        return nullptr;
    }

    return new ast::IntegerLiteral(currentToken, val);
}

ast::Expression *Parser::parsePrefixExpression()
{
    token::Token token = currentToken;
    std::string op = currentToken.literal;

    nextToken();
    ast::Expression *right = parseExpression(Precedence::PREFIX);

    return new ast::PrefixExpression(token, op, right);
}

void Parser::registerPrefix(token::TokenType tokenType, prefixParseFn fn)
{
    prefixParseFunctions[tokenType] = fn;
}

void Parser::registerInfix(token::TokenType tokenType, infixParseFn fn)
{
    infixParseFunctions[tokenType] = fn;
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
        peekError(type);
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
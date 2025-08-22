#include "parser.hpp"
#include <iostream>
#include <sstream>

// Mapping each operator to its precedence
std::unordered_map<token::TokenType, Precedence> precedences{
    {token::EQ, Precedence::EQUALS},
    {token::NOT_EQ, Precedence::EQUALS},
    {token::LT, Precedence::LESSGREATER},
    {token::GT, Precedence::LESSGREATER},
    {token::PLUS, Precedence::SUM},
    {token::MINUS, Precedence::SUM},
    {token::SLASH, Precedence::PRODUCT},
    {token::ASTERISK, Precedence::PRODUCT},
};

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
    // Reminder: Call to times cus in the first call
    // peekToken is empty and is assigned to currentToken
    nextToken();
    nextToken();

    registerPrefix(token::IDENT,
                   [this]
                   { return this->parseIdentifier(); });

    registerPrefix(token::INT,
                   [this]
                   { return this->parseIntegerLiteral(); });

    registerPrefix(token::TRUE,
                   [this]
                   { return this->parseBoolean(); });
    registerPrefix(token::FALSE,
                   [this]
                   { return this->parseBoolean(); });

    registerPrefix(token::LPAREN,
                   [this]
                   { return this->parseGroupedExpression(); });

    auto prefixParser = [this]
    {
        return this->parsePrefixExpression();
    };

    registerPrefix(token::BANG, prefixParser);
    registerPrefix(token::MINUS, prefixParser);

    auto infixParser = [this](ast::Expression *left)
    {
        return this->parseInfixExpression(left);
    };

    registerInfix(token::PLUS, infixParser);
    registerInfix(token::MINUS, infixParser);
    registerInfix(token::SLASH, infixParser);
    registerInfix(token::ASTERISK, infixParser);
    registerInfix(token::EQ, infixParser);
    registerInfix(token::NOT_EQ, infixParser);
    registerInfix(token::LT, infixParser);
    registerInfix(token::GT, infixParser);
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

    while (!peekTokenIs(token::SEMICOLON) && precedence < peekPrecedence())
    {
        infixParseFn infix = infixParseFunctions[peekToken.type];
        if (!infix)
        {
            return leftExpression;
        }

        nextToken();
        leftExpression = infix(leftExpression);
    }

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
    auto *expression = new ast::PrefixExpression(currentToken, currentToken.literal, nullptr);

    nextToken();
    expression->right = parseExpression(Precedence::PREFIX);

    return expression;
}

ast::Expression *Parser::parseInfixExpression(ast::Expression *left)
{
    auto *expression = new ast::InfixExpression(
        currentToken,
        left,
        currentToken.literal,
        nullptr);

    const Precedence precedence = currentPrecedence();
    nextToken();
    expression->right = parseExpression(precedence);

    return expression;
}

ast::Boolean *Parser::parseBoolean()
{
    return new ast::Boolean(currentToken, currentTokenIs(token::TRUE));
}

ast::Expression *Parser::parseGroupedExpression()
{
    nextToken();

    ast::Expression *expression = parseExpression(Precedence::LOWEST);

    if (!expectPeek(token::RPAREN))
    {
        return nullptr;
    }

    return expression;
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

Precedence Parser::peekPrecedence() const
{
    if (precedences.count(peekToken.type))
    {
        return precedences.at(peekToken.type);
    }

    return Precedence::LOWEST;
}

Precedence Parser::currentPrecedence() const
{
    if (precedences.count(currentToken.type))
    {
        return precedences.at(currentToken.type);
    }

    return Precedence::LOWEST;
}

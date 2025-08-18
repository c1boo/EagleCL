#pragma once

#include "token.hpp"
#include "lexer.hpp"
#include "ast.hpp"
#include <functional>

using prefixParseFn = std::function<ast::Expression *()>;
using infixParseFn = std::function<ast::Expression *(ast::Expression *)>;

enum class Precedence
{
    LOWEST,
    EQUALS,      // ==
    LESSGREATER, // < or >
    SUM,         // +
    PRODUCT,     // *
    PREFIX,      // !x or -X
    CALL,        // function call fun()
};

class Parser
{
private:
    std::vector<std::string> errors{};

    // Appends an error to errors stack if peek token is not the expected token
    void peekError(token::TokenType type);

    void noPrefixParseFnError(token::TokenType type);

public:
    lexer::Lexer *lexer;

    token::Token currentToken;
    token::Token peekToken;

    std::unordered_map<token::TokenType, prefixParseFn> prefixParseFunctions;
    std::unordered_map<token::TokenType, infixParseFn> infixParseFunctions;

    Parser(lexer::Lexer *lexer);

    ~Parser()
    {
        delete lexer;
    }

    void nextToken();
    ast::Program *parseProgram();
    ast::Statement *parseStatement();
    ast::VarStatement *parseVarStatement();
    ast::ReturnStatement *parseReturnStatement();
    ast::ExpressionStatement *parseExpressionStatement();
    ast::Expression *parseExpression(Precedence precedence);
    ast::Expression *parseIdentifier();
    ast::Expression *parseIntegerLiteral();
    ast::Expression *parsePrefixExpression();
    ast::Expression *parseInfixExpression(ast::Expression *left);

    // Registers the prefix parsing function for the given token type
    void registerPrefix(token::TokenType tokenType, prefixParseFn fn);

    // Register the infix parsing function for the given token type
    void registerInfix(token::TokenType tokenType, infixParseFn fn);

    // checks the type of the peekToken and only if the type is correct does it
    // advances the tokens by calling nextToken
    bool expectPeek(token::TokenType type);

    // checks whether the peek token is the given type
    bool peekTokenIs(token::TokenType type);

    // checks whether the current token is the given type
    bool currentTokenIs(token::TokenType type);

    // Returns the peek tokens precedence
    Precedence peekPrecedence() const;

    // Returns the current tokens precedence
    Precedence currentPrecedence() const;

    // Getter for happened errors while parsing
    std::vector<std::string> getErrors() { return errors; }
};

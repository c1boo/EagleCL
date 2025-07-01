#include "lexer.hpp"
#include <iostream>

using namespace lexer;

// Reads a char from input
void Lexer::readChar()
{
    // TODO: Support Unicode and UTF-8
    if (readPosition >= input.size())
    {
        ch = 0;
    }
    else
    {
        ch = input[readPosition];
    }
    position = readPosition;
    readPosition += 1;
}

char Lexer::peekChar()
{
    if (readPosition >= input.size())
    {
        return 0;
    }

    return input[readPosition];
}

// Returns the next token from the input buffer, and advances the position to the next char
token::Token Lexer::nextToken()
{
    skipWhitespace();

    token::Token token{};

    switch (ch)
    {
    case '=':
        if (peekChar() == '=')
        {
            token = {token::EQ, makeTwoCharToken()};
        }
        else
        {
            token = {newToken(token::ASSIGN)};
        }

        break;
    case ';':
        token = newToken(token::SEMICOLON);
        break;
    case '(':
        token = newToken(token::LPAREN);
        break;
    case ')':
        token = newToken(token::RPAREN);
        break;
    case '{':
        token = newToken(token::LBRACE);
        break;
    case '}':
        token = newToken(token::RBRACE);
        break;
    case ',':
        token = newToken(token::COMMA);
        break;
    case '+':
        token = newToken(token::PLUS);
        break;
    case '-':
        token = newToken(token::MINUS);
        break;
    case '!':
        if (peekChar() == '=')
        {
            token = {token::NOT_EQ, makeTwoCharToken()};
        }
        else
        {
            token = {newToken(token::BANG)};
        }

        break;
    case '*':
        token = newToken(token::ASTERISK);
        break;
    case '/':
        token = newToken(token::SLASH);
        break;
    case '<':
        if (peekChar() == '=')
        {
            token = {token::LT_EQ, makeTwoCharToken()};
        }
        else
        {
            token = {newToken(token::LT)};
        }

        break;
    case '>':
        if (peekChar() == '=')
        {
            token = {token::GT_EQ, makeTwoCharToken()};
        }
        else
        {
            token = {newToken(token::GT)};
        }

        break;
    case 0:
        token.literal = "";
        token.type = token::EOF_;
        return token;
    default:
        // Get the available identifiers, if not found make the token TokenType::ILLEGAL type
        if (isLetter(ch))
        {
            token.literal = readIdentifier();
            token.type = token::LookupIdentifier(token.literal);
            return token;
        }
        else if (isDigit(ch))
        {
            token.type = token::INT;
            token.literal = readNumber();
            return token;
        }
        else
        {
            token = newToken(token::ILLEGAL);
        }
        break;
    }

    readChar();
    return token;
}

// Returns new token from given type and the current char
token::Token Lexer::newToken(token::TokenType type)
{
    return {type, std::string(1, ch)};
}

std::string Lexer::makeTwoCharToken()
{
    char oldChar = ch;
    readChar();
    return std::string(1, oldChar) + std::string(1, ch);
}

bool Lexer::isLetter(char ch)
{
    return ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z') || ch == '_';
}

std::string Lexer::readIdentifier()
{
    size_t oldPosition = position;
    while (isLetter(ch))
    {
        readChar();
    }

    return input.substr(oldPosition, position - oldPosition);
}

bool Lexer::isDigit(char ch)
{
    return '0' <= ch && ch <= '9';
}

std::string Lexer::readNumber()
{
    size_t oldPosition = position;
    while (isDigit(ch))
    {
        readChar();
    }

    return input.substr(oldPosition, position - oldPosition);
}

void Lexer::skipWhitespace()
{
    while (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r')
    {
        readChar();
    }
}

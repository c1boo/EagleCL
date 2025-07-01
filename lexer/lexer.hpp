#pragma once

#include <string>
#include "token.hpp"

namespace lexer
{
    struct Lexer
    {
        std::string input{};
        size_t position{};     // Current position in input (points to current char)
        size_t readPosition{}; // Current reading position in input (after the current char)
        char ch{};             // Current char under examination

        Lexer(const std::string &in) : input(in)
        {
            readChar();
        }

        // Reads a char from input
        void readChar();

        // Peeks on the character in front of the cursor, works similiar to readChar but doesn't move the cursor
        // Returns the peeked char
        char peekChar();

        // Returns the next token from the input buffer, and advances the position to the next char
        token::Token nextToken();

        // Returns new token from given type and the current char
        token::Token newToken(token::TokenType type);

        // Makes a two character token from the current position of the cursor and the next character proceeding it
        std::string makeTwoCharToken();

        // Checks if the given char is a letter and returns a boolean accordingly
        bool isLetter(char ch);
        // Returns the next identifier in the input buffer
        std::string readIdentifier();

        // Checks if the given char is a digit
        bool isDigit(char ch);
        // Returns the next numebr in the input buffer;
        std::string readNumber();

        // Skips whitespaces in the input buffer;
        void skipWhitespace();
    };
}

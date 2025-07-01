#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

namespace token
{
    using TokenType = std::string_view;

    // Status
    constexpr TokenType ILLEGAL = "ILLEGAL";
    constexpr TokenType EOF_ = "EOF";

    // Identifiers
    constexpr TokenType IDENT = "IDENT";
    constexpr TokenType INT = "INT";

    // Operators
    constexpr TokenType ASSIGN = "=";
    constexpr TokenType PLUS = "+";
    constexpr TokenType MINUS = "-";
    constexpr TokenType BANG = "!";
    constexpr TokenType ASTERISK = "*";
    constexpr TokenType SLASH = "/";
    constexpr TokenType LT = "<";
    constexpr TokenType GT = ">";
    constexpr TokenType EQ = "==";
    constexpr TokenType NOT_EQ = "!=";
    constexpr TokenType LT_EQ = "<=";
    constexpr TokenType GT_EQ = ">=";

    // Delimiters
    constexpr TokenType COMMA = ",";
    constexpr TokenType SEMICOLON = ";";
    constexpr TokenType LPAREN = "(";
    constexpr TokenType RPAREN = ")";
    constexpr TokenType LBRACE = "{";
    constexpr TokenType RBRACE = "}";

    // Keywords
    constexpr TokenType FUNCTION = "FUNCTION";
    constexpr TokenType VAR = "VAR";
    constexpr TokenType TRUE = "TRUE";
    constexpr TokenType FALSE = "FALSE";
    constexpr TokenType IF = "IF";
    constexpr TokenType ELSE = "ELSE";
    constexpr TokenType RETURN = "RETURN";

    extern std::unordered_map<std::string_view, TokenType> keywords;

    struct Token
    {
        TokenType type;
        std::string literal;
    };

    // Returns the TokenType for the given identifier.
    // If the identifier is a keywords it return one of the available keyword TokenType
    // If not it returns TokenType::IDENT
    TokenType LookupIdentifier(std::string_view identifier);

}

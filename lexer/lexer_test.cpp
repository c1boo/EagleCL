#include <string>
#include <vector>
#include <iostream>
#include <assert.h>
#include "lexer.hpp"
#include "token.hpp"

struct Expected
{
    std::string expectedType{};
    std::string expectedLiteral{};
};

void TestNextToken()
{
    std::string input = R"(var a = 5;
var b = 10;

var add = funkcion(x, y) {
    x + y;  
};

var result = add(a, b);
!-/*5;
5 < 10 > 5;
nese (vertet) {
    kthen vertet;
} perndryshe {
    kthen falso;
}

9 != 10;
10 == 10;
3 <= 5;
3 >= 5;
)";

    std::vector<Expected> tests = {
        {std::string(token::VAR), "var"},
        {std::string(token::IDENT), "a"},
        {std::string(token::ASSIGN), "="},
        {std::string(token::INT), "5"},
        {std::string(token::SEMICOLON), ";"},
        {std::string(token::VAR), "var"},
        {std::string(token::IDENT), "b"},
        {std::string(token::ASSIGN), "="},
        {std::string(token::INT), "10"},
        {std::string(token::SEMICOLON), ";"},
        {std::string(token::VAR), "var"},
        {std::string(token::IDENT), "add"},
        {std::string(token::ASSIGN), "="},
        {std::string(token::FUNCTION), "funkcion"},
        {std::string(token::LPAREN), "("},
        {std::string(token::IDENT), "x"},
        {std::string(token::COMMA), ","},
        {std::string(token::IDENT), "y"},
        {std::string(token::RPAREN), ")"},
        {std::string(token::LBRACE), "{"},
        {std::string(token::IDENT), "x"},
        {std::string(token::PLUS), "+"},
        {std::string(token::IDENT), "y"},
        {std::string(token::SEMICOLON), ";"},
        {std::string(token::RBRACE), "}"},
        {std::string(token::SEMICOLON), ";"},
        {std::string(token::VAR), "var"},
        {std::string(token::IDENT), "result"},
        {std::string(token::ASSIGN), "="},
        {std::string(token::IDENT), "add"},
        {std::string(token::LPAREN), "("},
        {std::string(token::IDENT), "a"},
        {std::string(token::COMMA), ","},
        {std::string(token::IDENT), "b"},
        {std::string(token::RPAREN), ")"},
        {std::string(token::SEMICOLON), ";"},
        {std::string(token::BANG), "!"},
        {std::string(token::MINUS), "-"},
        {std::string(token::SLASH), "/"},
        {std::string(token::ASTERISK), "*"},
        {std::string(token::INT), "5"},
        {std::string(token::SEMICOLON), ";"},
        {std::string(token::INT), "5"},
        {std::string(token::LT), "<"},
        {std::string(token::INT), "10"},
        {std::string(token::GT), ">"},
        {std::string(token::INT), "5"},
        {std::string(token::SEMICOLON), ";"},
        {std::string(token::IF), "nese"},
        {std::string(token::LPAREN), "("},
        {std::string(token::TRUE), "vertet"},
        {std::string(token::RPAREN), ")"},
        {std::string(token::LBRACE), "{"},
        {std::string(token::RETURN), "kthen"},
        {std::string(token::TRUE), "vertet"},
        {std::string(token::SEMICOLON), ";"},
        {std::string(token::RBRACE), "}"},
        {std::string(token::ELSE), "perndryshe"},
        {std::string(token::LBRACE), "{"},
        {std::string(token::RETURN), "kthen"},
        {std::string(token::FALSE), "falso"},
        {std::string(token::SEMICOLON), ";"},
        {std::string(token::RBRACE), "}"},
        {std::string(token::INT), "9"},
        {std::string(token::NOT_EQ), "!="},
        {std::string(token::INT), "10"},
        {std::string(token::SEMICOLON), ";"},
        {std::string(token::INT), "10"},
        {std::string(token::EQ), "=="},
        {std::string(token::INT), "10"},
        {std::string(token::SEMICOLON), ";"},
        {std::string(token::INT), "3"},
        {std::string(token::LT_EQ), "<="},
        {std::string(token::INT), "5"},
        {std::string(token::SEMICOLON), ";"},
        {std::string(token::INT), "3"},
        {std::string(token::GT_EQ), ">="},
        {std::string(token::INT), "5"},
        {std::string(token::SEMICOLON), ";"},
        {std::string(token::EOF_), ""},
    };
    lexer::Lexer *lex = new lexer::Lexer(input);

    for (size_t i = 0; i < tests.size(); i++)
    {

        token::Token token = lex->nextToken();

        std::cout << "---------- Test [" << i << "] ----------\n";
        std::cout << "TokenType::" << token.type << " == ExpectedType::" << tests[i].expectedType << " (TEST)\n";
        std::cout << "Literal::" << token.literal << " == ExpectedLiteral::" << tests[i].expectedLiteral << " (TEST)\n";

        assert(token.type == tests[i].expectedType);
        assert(token.literal == tests[i].expectedLiteral);

        std::cout << "               PASSED\n"
                  << std::endl;
    }

    std::cout << "All tests passed!";
}

int main()
{
    TestNextToken();
    return 0;
}

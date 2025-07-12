#include <iostream>
#include <ast.hpp>
#include <token.hpp>
#include <assert.h>

void testString()
{
    // var myVar = anotherVar;
    auto varStatement = ast::VarStatement(
        {token::VAR, "var"},
        new ast::Identifier({token::IDENT, "myVar"}, "myVar"),
        new ast::Identifier({token::IDENT, "anotherVar"}, "anotherVar"));
    auto *program = new ast::Program();
    program->statements.push_back(dynamic_cast<ast::Statement *>(&varStatement));

    assert(program->toString() == "var myVar = anotherVar;" && "program.toString() wrong!");
}

int main()
{
    testString();
    std::cout << "Passed!";
    return 0;
}
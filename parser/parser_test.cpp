#include <assert.h>
#include <iostream>
#include "parser.hpp"

void testVarStatement(ast::Statement *statement, std::string_view expectedIdentifier);

void testVarStatements()
{
    std::string input = R"(
    var x = 5;
    var y = 10;
    var foobar = 838383;
    )";

    auto *lexer = new lexer::Lexer(input);
    auto *parser = new Parser(lexer);

    auto *program = parser->parseProgram();

    assert(program != nullptr);
    assert(program->statements.size() == 3);

    std::vector<std::string_view> expectedIdentifiers = {"x", "y", "foobar"};

    for (int i = 0; i < program->statements.size(); i++)
    {
        std::cout << "Test for statement [" << i << "]";

        ast::Statement *statement = program->statements[i];
        testVarStatement(statement, expectedIdentifiers[i]);

        std::cout << "Passed" << std::endl;
    }
}

void testVarStatement(ast::Statement *statement, std::string_view expectedIdentifier)
{
    assert(statement->tokenLiteral() == "var" && "Token literal not var");

    ast::VarStatement *varStatement = dynamic_cast<ast::VarStatement *>(statement);
    assert(varStatement != nullptr && "Statement not VarStatement");

    assert(varStatement->name->value == expectedIdentifier && "Identifier name not correct");
}

int main()
{
    testVarStatements();
}
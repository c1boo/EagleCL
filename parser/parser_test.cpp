#include <assert.h>
#include <iostream>
#include "parser.hpp"

void testVarStatement(ast::Statement *statement, std::string_view expectedIdentifier);
void checkParserErrors(Parser *parser);

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
    checkParserErrors(parser);

    assert(program != nullptr);
    assert(program->statements.size() == 3);

    std::vector<std::string_view> expectedIdentifiers = {"x", "y", "foobar"};

    for (int i = 0; i < program->statements.size(); i++)
    {
        std::cout << "Test for statement [" << i << "]\n";

        ast::Statement *statement = program->statements[i];
        testVarStatement(statement, expectedIdentifiers[i]);

        std::cout << "Passed" << std::endl;
    }

    delete program;
    delete parser;
}

void testVarStatement(ast::Statement *statement, std::string_view expectedIdentifier)
{
    assert(statement->tokenLiteral() == "var" && "Token literal not var");

    ast::VarStatement *varStatement = dynamic_cast<ast::VarStatement *>(statement);
    assert(varStatement != nullptr && "Statement not VarStatement");

    assert(varStatement->name->value == expectedIdentifier && "Identifier name not correct");
}

void checkParserErrors(Parser *parser)
{
    auto errors = parser->getErrors();
    if (errors.size() == 0)
    {
        return;
    }

    std::cout << "Parser has " << errors.size() << " errors" << std::endl;
    for (auto error : errors)
    {
        std::cout << "Parser error: " << error << std::endl;
    }

    assert(false);
}

int main()
{
    testVarStatements();
    return 0;
}
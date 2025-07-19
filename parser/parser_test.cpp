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

void testReturnStatements()
{
    std::string input = R"(
    kthen 5;
    kthen 10;
    kthen 993322;)";

    auto *lexer = new lexer::Lexer(input);
    auto *parser = new Parser(lexer);

    auto *program = parser->parseProgram();
    checkParserErrors(parser);

    assert(program != nullptr);
    std::cout << "Program has " << program->statements.size() << " statements" << std::endl;
    assert(program->statements.size() == 3 && "Program has not correct number of statements");

    for (int i = 0; i < program->statements.size(); i++)
    {
        std::cout << "Test for statement [" << i << "]\n";

        ast::ReturnStatement *returnStatement = dynamic_cast<ast::ReturnStatement *>(program->statements[i]);
        assert(returnStatement != nullptr && "Statement not ReturnStatement");
        assert(returnStatement->tokenLiteral() == "kthen" && "Token literal not return");

        std::cout << "Passed" << std::endl;
    }
}

void testIdentifierExpression()
{
    std::string input = "foobar;";

    auto *lexer = new lexer::Lexer(input);
    auto *parser = new Parser(lexer);

    auto *program = parser->parseProgram();
    checkParserErrors(parser);

    assert(program != nullptr);
    assert(program->statements.size() == 1 && "Program doesn't have enough statements");

    auto *exp = dynamic_cast<ast::ExpressionStatement *>(program->statements[0]);
    assert(exp && "Statement is not an ast::ExpressionStatement");

    auto *ident = dynamic_cast<ast::Identifier *>(exp->value);
    assert(ident && "Expression not a ast::Identifier");
    assert(ident->value == "foobar" && "identifier name not foobar");
    assert(ident->tokenLiteral() == "foobar" && "Identifiers token literal not foobar");

    std::cout << "Passed!";
}

void testIntegerLiteral()
{
    std::string input = "5;";

    auto *lexer = new lexer::Lexer(input);
    auto *parser = new Parser(lexer);

    auto *program = parser->parseProgram();
    checkParserErrors(parser);

    assert(program != nullptr);
    assert(program->statements.size() == 1 && "Program doesn't have enough statements");

    auto *exp = dynamic_cast<ast::ExpressionStatement *>(program->statements[0]);
    assert(exp && "Statement is not an ast::ExpressionStatement");

    auto *ident = dynamic_cast<ast::IntegerLiteral *>(exp->value);
    assert(ident && "Expression not a ast::IntegerLiteral");
    assert(ident->value == 5 && "literal value not 5");
    assert(ident->tokenLiteral() == "5" && "Identifiers token literal not 5");

    std::cout << "Passed!";
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
    // testVarStatements();
    // testReturnStatements();
    // testIdentifierExpression();
    testIntegerLiteral();

    return 0;
}
#include <assert.h>
#include <iostream>
#include "parser.hpp"

void testVarStatement(ast::Statement *statement, std::string_view expectedIdentifier);
void checkParserErrors(Parser *parser);

void testParseVarStatements()
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

void testParseReturnStatements()
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

void testParseIdentifierExpression()
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

    auto *ident = dynamic_cast<ast::Identifier *>(exp->expression);
    assert(ident && "Expression not a ast::Identifier");
    assert(ident->value == "foobar" && "identifier name not foobar");
    assert(ident->tokenLiteral() == "foobar" && "Identifiers token literal not foobar");

    std::cout << "Passed!";
}

void testParseIntegerLiteral()
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

    auto *ident = dynamic_cast<ast::IntegerLiteral *>(exp->expression);
    assert(ident && "Expression not a ast::IntegerLiteral");
    assert(ident->value == 5 && "literal value not 5");
    assert(ident->tokenLiteral() == "5" && "Identifiers token literal not 5");

    std::cout << "Passed!";
}

struct prefixTestCase
{
    std::string input;
    std::string op;
    int64_t value;
};

void testIntegerLiteral(ast::Expression *expression, int64_t value)
{
    auto *intLiteral = dynamic_cast<ast::IntegerLiteral *>(expression);
    assert(intLiteral && "intLiteral not a ast::IntegerLiteral");
    assert(intLiteral->value == value && "Value is not equal to the expected value");
    assert(intLiteral->tokenLiteral() != ("" + value) && "Token type mismatch");
}

void testParsePrefixExpression()
{
    std::vector<prefixTestCase> tests{
        {"!5;", "!", 5},
        {"-15;", "-", 15}};

    for (prefixTestCase test : tests)
    {
        auto *lexer = new lexer::Lexer(test.input);
        auto *parser = new Parser(lexer);
        auto *program = parser->parseProgram();
        checkParserErrors(parser);

        assert(program != nullptr);
        assert(program->statements.size() == 1 && "Program doesn't have enough statements! ");

        auto *statement = dynamic_cast<ast::ExpressionStatement *>(program->statements[0]);
        assert(statement && "Program statement is not an ast::ExpressionStatement");

        auto *expression = dynamic_cast<ast::PrefixExpression *>(statement->expression);
        assert(expression && "Statement is not a ast::PrefixExpression!");

        assert(expression->op == test.op && "Expression operator doesn't match with the expected operator");

        testIntegerLiteral(expression->right, test.value);

        std::cout << "Passed for expression: " << test.input << '\n';
    }
    std::cout << "PASSED!" << std::endl;
}

struct infixTestCase
{
    std::string input;
    int64_t leftOperand;
    std::string op;
    int64_t rightOperand;
};

void testParseInfixExpression()
{
    std::vector<infixTestCase> tests{
        {"5 + 5;", 5, "+", 5},
        {"5 - 5;", 5, "-", 5},
        {"5 * 5;", 5, "*", 5},
        {"5 / 5;", 5, "/", 5},
        {"5 < 5;", 5, "<", 5},
        {"5 > 5;", 5, ">", 5},
        {"5 == 5;", 5, "==", 5},
        {"5 != 5;", 5, "!=", 5},
    };

    std::cout << "----------[INFIX PARSE TEST]----------\n";
    for (infixTestCase test : tests)
    {
        std::cout << "TEST: " << test.input;

        auto *lexer = new lexer::Lexer(test.input);
        auto *parser = new Parser(lexer);
        auto *program = parser->parseProgram();
        checkParserErrors(parser);

        assert(program != nullptr);
        assert(program->statements.size() == 1 && "Program doesn't have enough statements");

        auto *statement = dynamic_cast<ast::ExpressionStatement *>(program->statements[0]);
        assert(statement && "Program statement is not an ast::ExpressionStatement");

        auto *expression = dynamic_cast<ast::InfixExpression *>(statement->expression);
        assert(expression && "Expression is not a ast::InfixExpression");

        testIntegerLiteral(expression->left, test.leftOperand);
        assert(expression->op == test.op && "Expression operator doesn't match with the expected operator");
        testIntegerLiteral(expression->right, test.rightOperand);

        std::cout << "\tPASSED!\n";
    };

    std::cout << "ALL INFIX TESTS PASSED!\n";
    std::cout << "-------------------------------------" << std::endl;
}

struct operatorPrecedenceTestCase
{
    std::string input;
    std::string expectedOutput;
};

void testOperatorPrecedenceParsing()
{
    std::vector<operatorPrecedenceTestCase> tests{
        {
            "-a * b",
            "((-a) * b)",
        },
        {
            "!-a",
            "(!(-a))",
        },
        {
            "a + b + c",
            "((a + b) + c)",
        },
        {
            "a + b - c",
            "((a + b) - c)",
        },
        {
            "a * b * c",
            "((a * b) * c)",
        },
        {
            "a * b / c",
            "((a * b) / c)",
        },
        {
            "a + b / c",
            "(a + (b / c))",
        },
        {
            "a + b * c + d / e - f",
            "(((a + (b * c)) + (d / e)) - f)",
        },
        {
            "3 + 4; -5 * 5",
            "(3 + 4)((-5) * 5)",
        },
        {
            "5 > 4 == 3 < 4",
            "((5 > 4) == (3 < 4))",
        },
        {
            "5 < 4 != 3 > 4",
            "((5 < 4) != (3 > 4))",
        },
        {
            "3 + 4 * 5 == 3 * 1 + 4 * 5",
            "((3 + (4 * 5)) == ((3 * 1) + (4 * 5)))",
        },
        {
            "3 + 4 * 5 == 3 * 1 + 4 * 5",
            "((3 + (4 * 5)) == ((3 * 1) + (4 * 5)))",
        },
    };

    std::cout << "-------------[Operator Precedence Test]------------\n";
    for (auto test : tests)
    {
        std::cout << "TEST: " << test.input;

        auto *lexer = new lexer::Lexer(test.input);
        auto *parser = new Parser(lexer);
        auto *program = parser->parseProgram();
        checkParserErrors(parser);

        std::string actualOutpu = program->toString();
        assert(actualOutpu == test.expectedOutput && "Not expected output.");

        std::cout << "\tPASSED!\n";
    }
    std::cout << "\t ALL OPERATOR PRECEDENCE TESTS PASSED!\n";
    std::cout << "---------------------------------------------------" << std::endl;
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
    // testParseVarStatements();
    // testParseReturnStatements();
    // testParseIdentifierExpression();
    // testParseIntegerLiteral();
    // testParsePrefixExpression();
    // testParseInfixExpression();
    testOperatorPrecedenceParsing();

    return 0;
}
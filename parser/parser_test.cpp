#include <assert.h>
#include <iostream>
#include "parser.hpp"
#include <any>

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

void testIntegerLiteral(ast::Expression *expression, int64_t expected)
{
    auto *intLiteral = dynamic_cast<ast::IntegerLiteral *>(expression);
    assert(intLiteral && "intLiteral not a ast::IntegerLiteral");
    assert(intLiteral->value == expected && "Value is not equal to the expected value");
    assert(intLiteral->tokenLiteral() != ("" + expected) && "Token type mismatch");
}

void testBooleanLiteral(ast::Expression *expression, bool expected)
{
    auto *boolExpression = dynamic_cast<ast::Boolean *>(expression);
    assert(boolExpression && "Expression is not a ast::Boolean*");
    assert(boolExpression->value == expected && "value is not expected value");
}

void testIdentifier(ast::Expression *expression, std::string expected)
{
    auto *identifier = dynamic_cast<ast::Identifier *>(expression);
    assert(identifier && "Expression is not a ast::Identifier*");
    assert(identifier->value == expected && "identifier.value is not equal to the expected value");
    assert(identifier->tokenLiteral() == expected && "identifier.tokenLiteral() is not equal to the expected value");
}

void testVarStatement(ast::Statement *statement, std::string_view expectedIdentifier)
{
    assert(token::keywords[statement->tokenLiteral()] == token::VAR && "Token literal not var");

    ast::VarStatement *varStatement = dynamic_cast<ast::VarStatement *>(statement);
    assert(varStatement && "Statement not ast::VarStatement*");

    assert(varStatement->name->value == expectedIdentifier && "Identifier name not correct");
}

void testLiteralExpression(ast::Expression *expression, std::any expected)
{
    const type_info &expectedType = expected.type();

    if (expectedType == typeid(int))
    {
        int x = std::any_cast<int>(expected);
        testIntegerLiteral(expression, static_cast<int64_t>(x));
    }
    else if (expectedType == typeid(int64_t))
    {
        testIntegerLiteral(expression, std::any_cast<int64_t>(expected));
    }
    else if (expectedType == typeid(std::string))
    {
        testIdentifier(expression, std::any_cast<std::string>(expected));
    }
    else if (expectedType == typeid(bool))
    {
        testBooleanLiteral(expression, std::any_cast<bool>(expected));
    }
    else
    {
        assert("type of expected not supported");
    }
}

void testInfixExpression(ast::Expression *expression, std::any left, std::string op, std::any right)
{
    auto *infixExpression = dynamic_cast<ast::InfixExpression *>(expression);
    assert(infixExpression && "expression is not a ast::InfixExpression*");

    testLiteralExpression(infixExpression->left, left);
    assert(infixExpression->op == op && "infixExpression operator does not match with the expected operator");
    testLiteralExpression(infixExpression->right, right);
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

    testIdentifier(exp->expression, "foobar");

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

    auto *expStatement = dynamic_cast<ast::ExpressionStatement *>(program->statements[0]);
    assert(expStatement && "Statement is not an ast::ExpressionStatement");

    testIntegerLiteral(expStatement->expression, 5);

    std::cout << "Passed!";
}

void testParseBooleanExpression()
{
    std::string input = "vertet;";

    auto *lexer = new lexer::Lexer(input);
    auto *parser = new Parser(lexer);

    auto *program = parser->parseProgram();
    checkParserErrors(parser);

    assert(program != nullptr);
    assert(program->statements.size() == 1 && "Program doesn't have enough statements");

    auto *expStatement = dynamic_cast<ast::ExpressionStatement *>(program->statements[0]);
    assert(expStatement && "Statement is not an ast::ExpressionStatement");

    testBooleanLiteral(expStatement->expression, true);

    std::cout << "Passed!" << std::endl;
}

void testParseIfExpression()
{
    std::string input = "nese (x < y) { x; }";

    std::cout << "-------------[If Expression Test]------------\n";
    auto *lexer = new lexer::Lexer(input);
    auto *parser = new Parser(lexer);
    auto *program = parser->parseProgram();
    checkParserErrors(parser);

    assert(program->statements.size() == 1 && "Program doesn't have enough statements");

    auto *expStatement = dynamic_cast<ast::ExpressionStatement *>(program->statements[0]);
    assert(expStatement && "expStatement is not a ast::ExpressionStatement*");

    auto *expression = dynamic_cast<ast::IfExpression *>(expStatement->expression);
    assert(expression && "expression is not a ast::IfExpression*");
    testInfixExpression(expression->condition, "x", "<", "y");

    assert(expression->consequence->statements.size() == 1 && "consequence size is not 1 statement");
    auto *consequence = dynamic_cast<ast::ExpressionStatement *>(expression->consequence->statements[0]);
    assert(consequence && "statements[0] is not a ast::ExpressionStatement*");
    testIdentifier(consequence->expression, "x");

    assert(!expression->alternative && "alternative.statements was not null when it should be");

    std::cout << "\t ALL IF EXPRESSION TESTS PASSED!\n";
    std::cout << "---------------------------------------------------" << std::endl;
}

void testParseIfElseExpression()
{
    std::string input = "nese (x < y) { x; } perndryshe {y;}";

    std::cout << "-------------[If Else Expression Test]------------\n";
    auto *lexer = new lexer::Lexer(input);
    auto *parser = new Parser(lexer);
    auto *program = parser->parseProgram();
    checkParserErrors(parser);

    assert(program->statements.size() == 1 && "Program doesn't have enough statements");

    auto *expStatement = dynamic_cast<ast::ExpressionStatement *>(program->statements[0]);
    assert(expStatement && "expStatement is not a ast::ExpressionStatement*");

    auto *expression = dynamic_cast<ast::IfExpression *>(expStatement->expression);
    assert(expression && "expression is not a ast::IfExpression*");
    testInfixExpression(expression->condition, "x", "<", "y");

    assert(expression->consequence->statements.size() == 1 && "consequence size is not 1 statement");
    auto *consequence = dynamic_cast<ast::ExpressionStatement *>(expression->consequence->statements[0]);
    assert(consequence && "statements[0] is not a ast::ExpressionStatement*");
    testIdentifier(consequence->expression, "x");


    assert(expression->alternative->statements.size() == 1 && "alternative size is not 1 statement");
    auto *alternative = dynamic_cast<ast::ExpressionStatement *>(expression->alternative->statements[0]);
    assert(alternative && "alternative is not a ast::ExpressionStatement*");
    testIdentifier(alternative->expression, "y");

    std::cout << "\t ALL IF ELSE EXPRESSION TESTS PASSED!\n";
    std::cout << "---------------------------------------------------" << std::endl;
}

struct VarTestCase
{
    std::string input;
    std::string expectedIdentifier;
    std::any expectedValue;
};

void testParseVarStatements()
{
    std::vector<VarTestCase> tests{
        {"var x = 5;", "x", 5},
        {"var y = 10;", "y", 10},
        {"var foobar = y;", "foobar", "y"},
    };

    std::cout << "----------[VAR STATEMENT PARSE TEST]----------\n";
    for (auto test : tests)
    {
        std::cout << "TEST: " << test.input;
        auto *lexer = new lexer::Lexer(test.input);
        auto *parser = new Parser(lexer);

        auto *program = parser->parseProgram();
        checkParserErrors(parser);

        assert(program != nullptr && "Program Fail!");
        assert(program->statements.size() == 1 && "Statements size mismatch!");

        ast::Statement *statement = program->statements[0];
        testVarStatement(statement, test.expectedIdentifier);

        auto *varStatement = dynamic_cast<ast::VarStatement *>(statement);
        testLiteralExpression(varStatement->expression, test.expectedValue);

        std::cout << "\tPASSED!\n";
    }

    std::cout << "ALL VAR STATEMENTS PASSED!\n";
    std::cout << "--------------------------------------------" << std::endl;
}

struct ReturnTestCase
{
    std::string input;
    std::any expected;
};

void testParseReturnStatements()
{
    std::vector<ReturnTestCase> tests{
        {"kthen 5;", 5},
        {"kthen 10;", 10},
        {"kthen 993322;", 993322},
        {"kthen \"bukuroshe\"", "bukuroshe"},
    };

    std::cout << "----------[RETURN STATEMENT PARSE TEST]----------\n";
    for (auto test : tests)
    {
        std::cout << "TEST: " << test.input;

        auto *lexer = new lexer::Lexer(test.input);
        auto *parser = new Parser(lexer);

        auto *program = parser->parseProgram();
        checkParserErrors(parser);
        assert(program && "Program is null");
        assert(program->statements.size() == 1 && "Program does not have correct number of statements");

        auto *statement = program->statements[0];
        auto *returnStatement = dynamic_cast<ast::ReturnStatement *>(program->statements[0]);
        assert(returnStatement && "Statement is not a ast::ReturnStatement*");
        assert(token::keywords[returnStatement->tokenLiteral()] == token::RETURN && "Token literal is not \"kthen\"");

        testLiteralExpression(returnStatement->returnValue, test.expected);
        std::cout << "\tPASSED!\n";
    }

    std::cout << "ALL RETURN STATEMENTS PASSED!\n";
    std::cout << "--------------------------------------------" << std::endl;
}

struct PrefixTestCase
{
    std::string input;
    std::string op;
    std::any value;
};

void testParsePrefixExpression()
{
    std::vector<PrefixTestCase> tests{
        {"!5;", "!", 5},
        {"-15;", "-", 15},
        {"!vertet;", "!", true},
        {"!falso", "!", false},
    };

    std::cout << "----------[PREFIX PARSE TEST]----------\n";
    for (PrefixTestCase test : tests)
    {
        std::cout << "TEST: " << test.input;

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

        testLiteralExpression(expression->right, test.value);

        std::cout << "\tPASSED!\n";
    }
    std::cout << "PASSED!" << std::endl;
    std::cout << "ALL PREFIX TESTS PASSED!\n";
    std::cout << "-------------------------------------" << std::endl;
}

struct InfixTestCase
{
    std::string input;
    std::any leftOperand;
    std::string op;
    std::any rightOperand;
};

void testParseInfixExpression()
{
    std::vector<InfixTestCase> tests{
        {"5 + 5;", 5, "+", 5},
        {"5 - 5;", 5, "-", 5},
        {"5 * 5;", 5, "*", 5},
        {"5 / 5;", 5, "/", 5},
        {"5 < 5;", 5, "<", 5},
        {"5 > 5;", 5, ">", 5},
        {"5 == 5;", 5, "==", 5},
        {"5 != 5;", 5, "!=", 5},
        {"vertet == vertet", true, "==", true},
        {"vertet != falso", true, "!=", false},
        {"falso == falso", false, "==", false},
    };

    std::cout << "----------[INFIX PARSE TEST]----------\n";
    for (InfixTestCase test : tests)
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

        testLiteralExpression(expression->left, test.leftOperand);
        assert(expression->op == test.op && "Expression operator doesn't match with the expected operator");
        testLiteralExpression(expression->right, test.rightOperand);

        std::cout << "\tPASSED!\n";
    };

    std::cout << "ALL INFIX TESTS PASSED!\n";
    std::cout << "-------------------------------------" << std::endl;
}

struct OperatorPrecedenceTestCase
{
    std::string input;
    std::string expectedOutput;
};

void testOperatorPrecedenceParsing()
{
    std::vector<OperatorPrecedenceTestCase> tests{
        {
            "vertet",
            "vertet",
        },
        {
            "falso",
            "falso",
        },
        {
            "3 < 5 == vertet",
            "((3 < 5) == vertet)",
        },
        {
            "3 > 5 == falso",
            "((3 > 5) == falso)",
        },
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
        {
            "1 + (2 + 3) + 4",
            "((1 + (2 + 3)) + 4)",
        },
        {
            "(5 + 5) * 2",
            "((5 + 5) * 2)",
        },
        {
            "2 / (5 + 5)",
            "(2 / (5 + 5))",
        },
        {
            "-(5 + 5)",
            "(-(5 + 5))",
        },
        {
            "!(vertet == vertet)",
            "(!(vertet == vertet))",
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

        std::string actualOutput = program->toString();
        assert(actualOutput == test.expectedOutput && "Not expected output.");

        std::cout << "\tPASSED!\n";
    }
    std::cout << "\t ALL OPERATOR PRECEDENCE TESTS PASSED!\n";
    std::cout << "---------------------------------------------------" << std::endl;
}

int main()
{
    //  testParseReturnStatements();
    //  testParseIdentifierExpression();
    //  testParseIntegerLiteral();
    testParsePrefixExpression();
    testParseInfixExpression();
    testOperatorPrecedenceParsing();
    testParseBooleanExpression();
    testParseIfExpression();
    testParseIfElseExpression();

    return 0;
}
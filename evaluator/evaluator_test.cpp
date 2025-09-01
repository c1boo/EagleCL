#include <assert.h>
#include <string>
#include <iostream>
#include "lexer.hpp"
#include "parser.hpp"
#include "object.hpp"
#include "evaluator.hpp"

object::Object *testEvaluate(std::string input)
{
    auto lexer = new lexer::Lexer(input);
    auto parser = new Parser(lexer);
    ast::Program *program = parser->parseProgram();

    return evaluator::evaluate(program);
}

void testIntegerObject(object::Object *obj, int64_t expectedInteger)
{
    const object::Integer *integer = dynamic_cast<object::Integer *>(obj);
    assert(integer && "object is not an object::Integer*");
    assert(integer->value == expectedInteger && "object has wrong value");
    delete integer;
}

void testEvalIntegerExpression()
{
    const std::vector<std::pair<std::string, int64_t>> tests{
        {"5", 5},
        {"10", 10},
        {"-5", -5},
        {"-15", -15},
        {"5 + 5 + 5 + 5 - 10", 10},
        {"2 * 2 * 2 * 2 * 2", 32},
        {"-50 + 100 + -50", 0},
        {"5 * 2 + 10", 20},
        {"5 + 2 * 10", 25},
        {"20 + 2 * -10", 0},
        {"50 / 2 * 2 + 10", 60},
        {"2 * (5 + 10)", 30},
        {"3 * 3 * 3 + 10", 37},
        {"3 * (3 * 3) + 10", 37},
        {"(5 + 10 * 2 + 15 / 3) * 2 + -10", 50},
    };

    for (const auto &test : tests)
    {
        object::Object *evaluated = testEvaluate(test.first);
        testIntegerObject(evaluated, test.second);
    }
}

void testBooleanObject(object::Object *obj, bool expectedBoolean)
{
    const object::Boolean *boolean = dynamic_cast<object::Boolean *>(obj);
    assert(boolean && "object is not an object::Boolean*");
    assert(boolean->value == expectedBoolean && "object has wrong value");
    delete boolean;
}

void testEvalBooleanExpression()
{
    const std::vector<std::pair<std::string, bool>> tests{
        {"vertet", true},
        {"falso", false},
        {"1 < 2", true},
        {"1 > 2", false},
        {"1 < 1", false},
        {"1 > 1", false},
        {"1 == 1", true},
        {"1 != 1", false},
        {"1 == 2", false},
        {"1 != 2", true},
        {"vertet == vertet", true},
        {"falso == falso", true},
        {"vertet == falso", false},
        {"vertet != falso", true},
        {"falso != vertet", true},
        {"(1 < 2) == vertet", true},
        {"(1 < 2) == falso", false},
        {"(1 > 2) == vertet", false},
        {"(1 > 2) == falso", true},
    };

    for (const auto &test : tests)
    {
        object::Object *evaluated = testEvaluate(test.first);
        testBooleanObject(evaluated, test.second);
    }
}

void testBangOperator()
{
    std::vector<std::pair<std::string, bool>> tests{
        {"!vertet", false},
        {"!falso", true},
        {"!5", false},
        {"!!vertet", true},
        {"!!falso", false},
        {"!!5", true},
    };

    for (const auto &test : tests)
    {
        object::Object *evaluated = testEvaluate(test.first);
        testBooleanObject(evaluated, test.second);
    }
}

int main()
{
    testEvalIntegerExpression();
    testEvalBooleanExpression();
    testBangOperator();

    std::cout << "EVALUATOR TESTS PASSED!" << std::endl;
}

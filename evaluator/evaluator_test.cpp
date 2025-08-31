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

    std::cout << "EVALUATOR TESTS PASSED!" << std::endl;
}

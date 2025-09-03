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
    auto env = new object::Environment();

    return evaluator::evaluate(program, env);
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

void testNullObject(object::Object *obj)
{
    const object::Null *null = dynamic_cast<object::Null *>(obj);
    assert(null && "object is not an object::Null*");
    delete null;
}

void testFunctionObject()
{
    std::string input = "funksion(x) { x + 2; };";

    auto *evaluated = testEvaluate(input);
    auto *func = dynamic_cast<object::Function *>(evaluated);

    assert(func && "func is not a object::Function*");
    assert(func->parameters.size() == 1 && "wrong parameters");
    assert(func->parameters[0]->toString() == "x" && "parameter is not 'x'");

    std::string expectedBody = "(x + 2)";
    assert(func->body->toString() == expectedBody && "body is not x + 2");
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

void testIfElseExpression()
{
    std::vector<std::pair<std::string, int *>> tests{
        {"nese (vertet) { 10 }", new int(10)},
        {"nese  (falso) { 10 }", nullptr},
        {"nese (1) { 10 }", new int(10)},
        {"nese (1 < 2) { 10 }", new int(10)},
        {"nese (1 > 2) { 10 }", nullptr},
        {"nese (1 > 2) { 10 } perndryshe { 20 }", new int(20)},
        {"nese (1 < 2) { 10 } perndryshe { 20 }", new int(10)},
    };

    for (const auto &test : tests)
    {
        object::Object *evaluated = testEvaluate(test.first);
        if (test.second)
        {
            testIntegerObject(evaluated, *test.second);
        }
        else
        {
            testNullObject(evaluated);
        }
    }
}

void testReturnStatement()
{
    std::vector<std::pair<std::string, int>> tests{
        {"kthen 10;", 10},
        {"kthen 10; 9;", 10},
        {"kthen 2 * 5; 9;", 10},
        {"9; kthen 2 * 5; 9;", 10},
        {"nese (10 > 1) { nese (10 > 1) {kthen 10;} kthen 1;}", 10}};

    for (const auto &test : tests)
    {
        object::Object *evaluated = testEvaluate(test.first);
        testIntegerObject(evaluated, test.second);
    }
}

void testVarStatements()
{
    std::vector<std::pair<std::string, int64_t>> tests{
        {"var a = 5; a;", 5},
        {"var a = 5 * 5; a;", 25},
        {"var a = 5; var b = a; b;", 5},
        {"var a = 5; var b = a; var c = a + b + 5; c;", 15},
    };

    for (const auto &test : tests)
    {
        testIntegerObject(testEvaluate(test.first), test.second);
    }
}

void testFunctionCall()
{
    std::vector<std::pair<std::string, int64_t>> tests{
        {"var identity = funksion(x) { x; }; identity(5);", 5},
        {"var identity = funksion(x) { kthen x; }; identity(5);", 5},
        {"var double = funksion(x) { x * 2; }; double(5);", 10},
        {"var add = funksion(x, y) { x + y; }; add(5, 5);", 10},
        {"var add = funksion(x, y) { x + y; }; add(5 + 5, add(5, 5));", 20},
        {"funksion(x) { x; }(5)", 5},
    };

    for (const auto &test : tests)
    {
        testIntegerObject(testEvaluate(test.first), test.second);
    }
}

void testClosure()
{
    std::string input = R"(
    var newAdder = funksion(x) {
        funksion(y) {
            x + y; 
        }
    }
    var addTwo = newAdder(2);
    addTwo(2);
    )";

    testIntegerObject(testEvaluate(input), 4);
}

void testErrorHandling()
{
    std::vector<std::pair<std::string, std::string>> tests{
        {"5 + vertet", "mospërputhje i tipit: INTEGJER + BOOLEAN"},
        {"5 + vertet; 5;", "mospërputhje i tipit: INTEGJER + BOOLEAN"},
        {"-vertet", "operator i panjohur: - BOOLEAN"},
        {"vertet + falso", "operator i panjohur: BOOLEAN + BOOLEAN"},
        {"5; vertet + falso; 5", "operator i panjohur: BOOLEAN + BOOLEAN"},
        {"nese (10 > 1) {vertet + falso}", "operator i panjohur: BOOLEAN + BOOLEAN"},
        {"nese (10 > 1) { nese (10 > 1) {kthen vertet + falso;} kthen 1;}", "operator i panjohur: BOOLEAN + BOOLEAN"},
        {"foobar;", "identifikuesi nuk gjindet: foobar"},
    };

    for (const auto &test : tests)
    {
        object::Object *evaluated = testEvaluate(test.first);
        object::Error *errorObj = dynamic_cast<object::Error *>(evaluated);
        assert(errorObj && "no error object returned.");
        assert(errorObj->message == test.second && "wrong error message!.");
    }
}

int main()
{
    testEvalIntegerExpression();
    testEvalBooleanExpression();
    testBangOperator();
    testIfElseExpression();
    testReturnStatement();
    testVarStatements();
    testFunctionObject();
    testFunctionCall();
    testClosure();
    testErrorHandling();

    std::cout << "EVALUATOR TESTS PASSED!" << std::endl;
}

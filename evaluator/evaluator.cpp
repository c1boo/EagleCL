#include "evaluator.hpp"

object::Object *evaluator::evaluate(ast::Node *node)
{
    if (auto *program = dynamic_cast<ast::Program *>(node))
        return evalProgram(program->statements);

    if (auto *expStatement = dynamic_cast<ast::ExpressionStatement *>(node))
        return evaluate(expStatement->expression);

    if (auto *integer = dynamic_cast<ast::IntegerLiteral *>(node))
        return new object::Integer(integer->value);

    if (auto *boolean = dynamic_cast<ast::Boolean *>(node))
    {
        // NOTE: Booleans can be singletons in the future if GC is decided to be implemented
        return new object::Boolean(boolean->value);
    }

    if (auto *prefixExpression = dynamic_cast<ast::PrefixExpression *>(node))
    {
        object::Object *right = evaluate(prefixExpression->right);
        return evaluatePrefixExpression(prefixExpression->op, right);
    }

    if (auto *infixExpression = dynamic_cast<ast::InfixExpression *>(node))
    {
        object::Object *left = evaluate(infixExpression->left);
        object::Object *right = evaluate(infixExpression->right);
        return evaluateInfixExpression(infixExpression->op, left, right);
    }

    if (auto *blockStatement = dynamic_cast<ast::BlockStatement *>(node))
    {
        return evalBlockStatements(blockStatement->statements);
    }

    if (auto *ifStatement = dynamic_cast<ast::IfExpression *>(node))
    {
        return evaluateIfStatement(ifStatement);
    }

    if (auto *returnStatement = dynamic_cast<ast::ReturnStatement *>(node))
    {
        object::Object *returnVal = evaluate(returnStatement->returnValue);
        return new object::ReturnValue(returnVal);
    }

    return nullptr;
}

object::Object *evaluator::evalProgram(const std::vector<ast::Statement *> &statements)
{
    object::Object *result;

    for (auto *statement : statements)
    {
        result = evaluate(statement);

        if (auto *returnVal = dynamic_cast<object::ReturnValue *>(result))
        {
            return returnVal->value;
        }
    }

    return result;
}

object::Object *evaluator::evalBlockStatements(const std::vector<ast::Statement *> &statements)
{
    object::Object *result;

    for (auto *statement : statements)
    {
        result = evaluate(statement);

        if (result && result->type() == object::RETURN_VALUE_OBJ)
        {
            return result;
        }
    }

    return result;
}

object::Object *evaluator::evaluatePrefixExpression(std::string_view op, object::Object *rightExpression)
{
    switch (op.front())
    {
    case '!':
        return evaluateBangOperatorExpression(rightExpression);
    case '-':
        return evaluateMinusPrefixOperatorExpression(rightExpression);
    default:
        return new object::Null();
    }
}

object::Object *evaluator::evaluateBangOperatorExpression(object::Object *rightExpression)
{
    if (rightExpression == nullptr)
    {
        return new object::Boolean(true);
    }

    if (auto boolean = dynamic_cast<object::Boolean *>(rightExpression))
    {
        return new object::Boolean(!boolean->value);
    }

    return new object::Boolean(false);
}

object::Object *evaluator::evaluateMinusPrefixOperatorExpression(object::Object *rightExpression)
{
    if (rightExpression->type() != object::INTEGER_OBJ)
    {
        return new object::Null();
    }

    auto *integer = dynamic_cast<object::Integer *>(rightExpression);
    integer->value *= -1;

    return integer;
}

object::Object *evaluator::evaluateInfixExpression(std::string_view op, object::Object *left, object::Object *right)
{
    if (left->type() == object::INTEGER_OBJ && right->type() == object::INTEGER_OBJ)
    {
        return evaluateInfixIntegerExpression(op, left, right);
    }

    if (left->type() == object::BOOLEAN_OBJ && right->type() == object::BOOLEAN_OBJ)
    {
        return evaluateInfixBooleanExpression(op, left, right);
    }

    return new object::Null();
}

object::Object *evaluator::evaluateInfixIntegerExpression(std::string_view op, object::Object *left, object::Object *right)
{
    int64_t leftValue = dynamic_cast<object::Integer *>(left)->value;
    int64_t rightValue = dynamic_cast<object::Integer *>(right)->value;

    if (op == "+")
        return new object::Integer(leftValue + rightValue);
    if (op == "-")
        return new object::Integer(leftValue - rightValue);
    if (op == "*")
        return new object::Integer(leftValue * rightValue);
    if (op == "/")
        return new object::Integer(leftValue / rightValue);

    if (op == "<")
        return new object::Boolean(leftValue < rightValue);
    if (op == ">")
        return new object::Boolean(leftValue > rightValue);
    if (op == "==")
        return new object::Boolean(leftValue == rightValue);
    if (op == "!=")
        return new object::Boolean(leftValue != rightValue);
    if (op == "<=")
        return new object::Boolean(leftValue <= rightValue);
    if (op == ">=")
        return new object::Boolean(leftValue >= rightValue);

    return new object::Null();
}

object::Object *evaluator::evaluateInfixBooleanExpression(std::string_view op, object::Object *left, object::Object *right)
{
    bool leftVal = dynamic_cast<object::Boolean *>(left)->value;
    bool rightVal = dynamic_cast<object::Boolean *>(right)->value;

    if (op == "==")
        return new object::Boolean(leftVal == rightVal);
    if (op == "!=")
        return new object::Boolean(leftVal != rightVal);

    return new object::Null();
}

object::Object *evaluator::evaluateIfStatement(ast::IfExpression *statement)
{
    object::Object *condition = evaluate(statement->condition);

    if (isTruthy(condition))
    {
        return evaluate(statement->consequence);
    }
    else if (statement->alternative)
    {
        return evaluate(statement->alternative);
    }

    return new object::Null();
}

bool evaluator::isTruthy(object::Object *obj)
{
    bool condition = true;
    if (dynamic_cast<object::Null *>(obj))
    {
        condition = false;
    }
    else if (auto *b = dynamic_cast<object::Boolean *>(obj))
    {
        condition = b->value;
    }

    return condition;
}

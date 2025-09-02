#include "evaluator.hpp"

object::Object *evaluator::evaluate(ast::Node *node, object::Environment *env)
{
    // Statements
    if (auto *program = dynamic_cast<ast::Program *>(node))
        return evaluateProgram(program->statements, env);

    if (auto *blockStatement = dynamic_cast<ast::BlockStatement *>(node))
        return evaluateBlockStatements(blockStatement->statements, env);

    if (auto *expStatement = dynamic_cast<ast::ExpressionStatement *>(node))
        return evaluate(expStatement->expression, env);

    if (auto *returnStatement = dynamic_cast<ast::ReturnStatement *>(node))
    {
        object::Object *returnVal = evaluate(returnStatement->returnValue, env);
        if (isError(returnVal))
        {
            return returnVal;
        }

        return new object::ReturnValue(returnVal);
    }

    if (auto *varStatement = dynamic_cast<ast::VarStatement *>(node))
    {
        object::Object *value = evaluate(varStatement->expression, env);
        if (isError(value))
        {
            return value;
        }

        env->set(varStatement->name->value, value);
    }

    // Expressions
    if (auto *integer = dynamic_cast<ast::IntegerLiteral *>(node))
        return new object::Integer(integer->value);

    if (auto *boolean = dynamic_cast<ast::Boolean *>(node))
        // NOTE: Booleans can be singletons in the future if GC is decided to be implemented
        return new object::Boolean(boolean->value);

    if (auto *prefixExpression = dynamic_cast<ast::PrefixExpression *>(node))
    {
        object::Object *right = evaluate(prefixExpression->right, env);
        if (isError(right))
        {
            return right;
        }

        return evaluatePrefixExpression(prefixExpression->op, right);
    }

    if (auto *infixExpression = dynamic_cast<ast::InfixExpression *>(node))
    {
        object::Object *left = evaluate(infixExpression->left, env);
        if (isError(left))
        {
            return left;
        }

        object::Object *right = evaluate(infixExpression->right, env);
        if (isError(right))
        {
            return right;
        }

        return evaluateInfixExpression(infixExpression->op, left, right);
    }

    if (auto *ifExpression = dynamic_cast<ast::IfExpression *>(node))
    {
        return evaluateIfStatement(ifExpression, env);
    }

    if (auto *identifier = dynamic_cast<ast::Identifier *>(node))
    {
        return evaluateIdentifier(identifier, env);
    }

    return nullptr;
}

object::Object *evaluator::evaluateProgram(const std::vector<ast::Statement *> &statements,
                                           object::Environment *env)
{
    object::Object *result;

    for (auto *statement : statements)
    {
        result = evaluate(statement, env);

        if (auto *returnVal = dynamic_cast<object::ReturnValue *>(result))
        {
            return returnVal->value;
        }

        if (auto *errorObj = dynamic_cast<object::Error *>(result))
        {
            return result;
        }
    }

    return result;
}

object::Object *evaluator::evaluateBlockStatements(const std::vector<ast::Statement *> &statements,
                                                   object::Environment *env)
{
    object::Object *result;

    for (auto *statement : statements)
    {
        result = evaluate(statement, env);

        if (result)
        {
            const auto &rt = result->type();
            if (rt == object::RETURN_VALUE_OBJ || rt == object::ERROR_OBJ)
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
        return newError(object::UNKNOWN_OP_ERR, op, rightExpression->type());
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
        return newError(object::UNKNOWN_OP_ERR, "-", rightExpression->type());
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

    if (left->type() != right->type())
    {
        return newError(object::TYPE_MISMATCH_ERR, left->type(), op, right->type());
    }

    return newError(object::UNKNOWN_OP_ERR, left->type(), op, right->type());
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

    return newError(object::UNKNOWN_OP_ERR, left->type(), op, right->type());
}

object::Object *evaluator::evaluateInfixBooleanExpression(std::string_view op,
                                                          object::Object *left,
                                                          object::Object *right)
{
    bool leftVal = dynamic_cast<object::Boolean *>(left)->value;
    bool rightVal = dynamic_cast<object::Boolean *>(right)->value;

    if (op == "==")
        return new object::Boolean(leftVal == rightVal);
    if (op == "!=")
        return new object::Boolean(leftVal != rightVal);

    return newError(object::UNKNOWN_OP_ERR, left->type(), op, right->type());
}

object::Object *evaluator::evaluateIfStatement(ast::IfExpression *expresssion,
                                               object::Environment *env)
{
    object::Object *condition = evaluate(expresssion->condition, env);

    if (isError(condition))
    {
        return condition;
    }

    if (isTruthy(condition))
    {
        return evaluate(expresssion->consequence, env);
    }
    else if (expresssion->alternative)
    {
        return evaluate(expresssion->alternative, env);
    }

    return new object::Null();
}

object::Object *evaluator::evaluateIdentifier(ast::Identifier *identifier, object::Environment *env)
{
    object::Object *val = env->get(identifier->value);

    if (!val)
    {
        return newError(object::UNKNOWN_IDENT, identifier->value);
    }

    return val;
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

bool evaluator::isError(object::Object *obj)
{
    if (obj)
    {
        return obj->type() == object::ERROR_OBJ;
    }

    return false;
}

#pragma once

#include "ast.hpp"
#include "object.hpp"
#include <vector>

namespace evaluator
{
    object::Object *evaluate(ast::Node *node);

    object::Object *evalProgram(const std::vector<ast::Statement *> &statements);

    object::Object *evalBlockStatements(const std::vector<ast::Statement *> &statements);

    object::Object *evaluatePrefixExpression(std::string_view op, object::Object *rightExpression);

    object::Object *evaluateBangOperatorExpression(object::Object *rightExpression);

    object::Object *evaluateMinusPrefixOperatorExpression(object::Object *rightExpression);

    object::Object *evaluateInfixExpression(std::string_view op,
                                            object::Object *left,
                                            object::Object *right);

    object::Object *evaluateInfixIntegerExpression(std::string_view op,
                                                   object::Object *left,
                                                   object::Object *right);

    object::Object *evaluateInfixBooleanExpression(std::string_view op,
                                                   object::Object *left,
                                                   object::Object *right);

    object::Object *evaluateIfStatement(ast::IfExpression *statement);

    bool isTruthy(object::Object *obj);
}
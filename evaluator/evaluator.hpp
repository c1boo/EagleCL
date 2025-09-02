#pragma once

#include <vector>
#include <sstream>
#include "ast.hpp"
#include "object.hpp"
#include "environment.hpp"

namespace evaluator
{
    object::Object *evaluate(ast::Node *node, object::Environment *env);

    object::Object *evaluateProgram(const std::vector<ast::Statement *> &statements,
                                    object::Environment *env);

    object::Object *evaluateBlockStatements(const std::vector<ast::Statement *> &statements,
                                            object::Environment *env);

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

    object::Object *evaluateIfStatement(ast::IfExpression *statement,
                                        object::Environment *env);

    object::Object *evaluateIdentifier(ast::Identifier *identifier,
                                       object::Environment *env);

    bool isTruthy(object::Object *obj);

    template <typename... Operands>
    object::Error *newError(std::string_view message, Operands... ops)
    {

        std::ostringstream oss;
        oss << message << ": ";
        ((oss << ops << " "), ...);

        std::string msg = oss.str();
        msg.pop_back(); // Delete last space
        return new object::Error(msg);
    }

    bool isError(object::Object *obj);
}
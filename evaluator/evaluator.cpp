#include "evaluator.hpp"

object::Null NULL_OBJ();

object::Object *evaluator::evaluate(ast::Node *node)
{
    if (auto *program = dynamic_cast<ast::Program *>(node))
        return evalStatements(program->statements);

    if (auto *expStatement = dynamic_cast<ast::ExpressionStatement *>(node))
        return evaluate(expStatement->expression);

    if (auto *integer = dynamic_cast<ast::IntegerLiteral *>(node))
        return new object::Integer(integer->value);

    if (auto *boolean = dynamic_cast<ast::Boolean *>(node))
    {
        // NOTE: Booleans can be singletons in the future if GC is decided to be implemented
        return new object::Boolean(boolean->value);
    }

    return nullptr;
}

object::Object *evaluator::evalStatements(const std::vector<ast::Statement *> &statements)
{
    object::Object *result;

    for (auto *statement : statements)
    {
        result = evaluate(statement);
    }

    return result;
}

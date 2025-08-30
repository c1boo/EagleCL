#pragma once

#include "ast.hpp"
#include "object.hpp"
#include <vector>

namespace evaluator
{
    object::Object *evalStatements(const std::vector<ast::Statement *> &statements);

    object::Object *evaluate(ast::Node *node);
}
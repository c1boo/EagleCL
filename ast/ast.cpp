#include "ast.hpp"
#include <sstream>

using namespace ast;

std::string Program::tokenLiteral() const
{
    if (statements.size() > 0 && statements[0])
    {
        return statements[0]->tokenLiteral();
    }
    else
    {
        return "";
    }
}

std::string Program::toString() const
{
    std::ostringstream oss;
    for (auto statement : statements)
    {
        oss << statement->toString();
    }

    return oss.str();
}

std::string VarStatement::toString() const
{
    std::ostringstream oss;
    oss << tokenLiteral() << " " << name->value << " = ";
    if (value)
    {
        oss << value->toString();
    }
    oss << ";";

    return oss.str();
}

std::string ReturnStatement::toString() const
{
    std::ostringstream oss;
    oss << tokenLiteral() << " ";
    if (value)
    {
        oss << value->toString();
    }
    oss << ";";

    return oss.str();
}

std::string ExpressionStatement::toString() const
{
    // TODO: Null check will be removed once expression parsing is proprely done
    if (value)
    {
        return value->toString();
    }

    return "";
}

std::string PrefixExpression::toString() const
{
    std::ostringstream oss;
    oss << "(" << op << right->toString() << ")";

    return oss.str();
}

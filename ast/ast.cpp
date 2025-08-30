#include "ast.hpp"
#include <sstream>

using namespace ast;

// Pushes elements of the given array to the string stream buffer with elements being seperated with commas
template <class T>
void pushElementsToBuffer(std::ostringstream &oss, const std::vector<T *> &arr)
{
    oss << "(";
    for (size_t i = 0; i < arr.size(); ++i)
    {
        oss << arr[i]->toString();
        if (i < arr.size() - 1)
        {
            oss << ", ";
        }
    }
    oss << ")";
}

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
    if (expression)
    {
        oss << expression->toString();
    }
    oss << ";";

    return oss.str();
}

std::string ReturnStatement::toString() const
{
    std::ostringstream oss;
    oss << tokenLiteral() << " ";
    if (returnValue)
    {
        oss << returnValue->toString();
    }
    oss << ";";

    return oss.str();
}

std::string ExpressionStatement::toString() const
{
    // TODO: Null check will be removed once expression parsing is proprely done
    if (expression)
    {
        return expression->toString();
    }

    return "";
}

std::string PrefixExpression::toString() const
{
    std::ostringstream oss;
    oss << "(" << op << right->toString() << ")";

    return oss.str();
}

std::string InfixExpression::toString() const
{
    std::ostringstream oss;
    oss << "(" << left->toString() << " " << op << " " << right->toString() << ")";

    return oss.str();
}

std::string IfExpression::toString() const
{
    std::ostringstream oss;
    oss << "nese" << condition->toString() << " " << consequence->toString();

    if (alternative)
    {
        oss << "perndryshe" << alternative->toString();
    }

    return oss.str();
}

std::string ast::BlockStatement::toString() const
{
    std::ostringstream oss;

    for (auto *statement : statements)
    {
        oss << statement->toString();
    }

    return oss.str();
}

std::string FunctionLiteral::toString() const
{
    std::ostringstream oss;
    oss << tokenLiteral();
    pushElementsToBuffer(oss, parameters);
    oss << body->toString();

    return oss.str();
}

std::string CallExpression::toString() const
{
    std::ostringstream oss;
    oss << function->toString();
    pushElementsToBuffer(oss, arguments);

    return oss.str();
}

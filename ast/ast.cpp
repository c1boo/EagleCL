#include "ast.hpp"

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

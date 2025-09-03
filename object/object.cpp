#include "object.hpp"
#include <sstream>

using namespace object;

ObjectType Integer::type() const
{
    return INTEGER_OBJ;
}

std::string Integer::inspect() const
{
    return std::to_string(value);
}

ObjectType Boolean::type() const
{
    return BOOLEAN_OBJ;
}

std::string Boolean::inspect() const
{
    std::ostringstream oss;
    oss << std::boolalpha << value;
    return oss.str();
}

ObjectType Null::type() const
{
    return NULL_OBJ;
}

std::string Null::inspect() const
{
    return "null";
}

ObjectType ReturnValue::type() const
{
    return RETURN_VALUE_OBJ;
}

std::string ReturnValue::inspect() const
{
    return value->inspect();
}

ObjectType Error::type() const
{
    return ERROR_OBJ;
}

std::string Error::inspect() const
{
    return "GABIM: " + message;
}

ObjectType Function::type() const
{
    return FUNC_OBJECT;
}

std::string Function::inspect() const
{
    std::ostringstream oss;

    oss << "funksion";

    oss << "(";
    for (size_t i = 0; i < parameters.size(); ++i)
    {
        if (i != 0)
            oss << ", ";

        oss << parameters[i]->toString();
    }
    oss << ") {\n";
    oss << body->toString();
    oss << "\n}";

    return oss.str();
}

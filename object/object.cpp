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

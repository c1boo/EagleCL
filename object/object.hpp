#pragma once
#include <string>

namespace object
{
    using ObjectType = std::string_view;

    // Data Types
    constexpr ObjectType INTEGER_OBJ = "INTEGER";
    constexpr ObjectType BOOLEAN_OBJ = "BOOLEAN";
    constexpr ObjectType NULL_OBJ = "NULL";
    constexpr ObjectType RETURN_VALUE_OBJ = "RETURN_VALUE";

    class Object
    {
    public:
        virtual ObjectType type() const = 0;
        virtual std::string inspect() const = 0;
    };

    class Integer : public Object
    {
    public:
        int64_t value;

        Integer() = default;
        Integer(int64_t val) : value{val} {};

        ObjectType type() const override;
        std::string inspect() const override;
    };

    class Boolean : public Object
    {
    public:
        bool value;

        Boolean() = default;
        Boolean(bool val) : value{val} {};

        ObjectType type() const override;
        std::string inspect() const override;
    };

    class Null : public Object
    {
    public:
        ObjectType type() const override;
        std::string inspect() const override;
    };

    class ReturnValue : public Object
    {
    public:
        Object *value;

        ReturnValue() = default;
        ReturnValue(Object *obj) : value{obj} {};

        ~ReturnValue()
        {
            delete value;
        }

        ObjectType type() const override;
        std::string inspect() const override;
    };

}
#pragma once

#include <unordered_map>
#include <string>
#include "object.hpp"

namespace object
{
    class Environment
    {
    public:
        std::unordered_map<std::string, Object *> store;

        Environment() = default;

        ~Environment()
        {
            for (auto el : store)
            {
                delete el.second;
            }
        }

        Environment(const Environment &) = delete;
        Environment &operator=(const Environment &) = delete;

        Object *get(const std::string &name) const;

        Object *set(const std::string &name, Object *value);
    };
}

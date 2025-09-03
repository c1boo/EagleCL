#include "environment.hpp"

using namespace object;

Object *Environment::get(const std::string &name) const
{
    if (!store.count(name))
    {
        if (outerEnvironment)
            return outerEnvironment->get(name);

        return nullptr;
    }

    return store.at(name);
}

Object *Environment::set(const std::string &name, Object *value)
{
    store.insert({name, value});
    return value;
}

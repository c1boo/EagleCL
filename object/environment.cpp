#include "environment.hpp"

using namespace object;

Object *Environment::get(const std::string &name) const
{
    return (store.count(name) ? store.at(name) : nullptr);
}

Object *Environment::set(const std::string &name, Object *value)
{
    store.insert({name, value});
    return value;
}

#pragma once

#include <iostream>

namespace repl
{
    constexpr std::string_view PROMPT = ">> ";

    void start(std::istream &in, std::ostream &out);
}
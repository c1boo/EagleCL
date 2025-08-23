#pragma once

#include <iostream>
#include <vector>

namespace repl
{
    constexpr std::string_view PROMPT = ">> ";

    void start(std::istream &in, std::ostream &out);

    void printParseErrors(std::ostream &out, const std::vector<std::string> &errors);
}
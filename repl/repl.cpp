#include "repl.hpp"

#include <string>
#include "lexer.hpp"
#include "token.hpp"

namespace repl
{

    void start(std::istream &in, std::ostream &out)
    {
        std::string line{};

        while (out << PROMPT && std::getline(in, line))
        {
            lexer::Lexer lex = lexer::Lexer(line);
            token::Token token = lex.nextToken();
            while (token.type != token::EOF_)
            {
                out << "Type: " << token.type << " Literal: " << token.literal << std::endl;
                token = lex.nextToken();
            }
        }
    }

}

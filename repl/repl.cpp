#include "repl.hpp"
#include "token.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "evaluator.hpp"

void repl::start(std::istream &in, std::ostream &out)
{
    std::string line{};

    while (out << PROMPT && std::getline(in, line))
    {
        auto *lexer = new lexer::Lexer(line);
        auto *parser = new Parser(lexer);
        auto *program = parser->parseProgram();

        const std::vector<std::string> errors = parser->getErrors();
        if (errors.size() != 0)
        {
            printParseErrors(out, errors);
            continue;
        }

        const object::Object *evaluatedStatement = evaluator::evaluate(program);
        if (evaluatedStatement)
        {
            out << evaluatedStatement->inspect();
            out << std::endl;
        }

        delete evaluatedStatement;
        delete program;
        delete parser;
    }
}

void repl::printParseErrors(std::ostream &out, const std::vector<std::string> &errors)
{
    for (const std::string &msg : errors)
    {
        out << "\t" << msg << "\n";
    }
    out << std::endl;
}

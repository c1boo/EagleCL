#include "token.hpp"
#include <stdexcept>

namespace token
{
    std::unordered_map<std::string_view, TokenType> keywords = {
        {"funksion", FUNCTION},
        {"var", VAR},
        {"vertet", TRUE},
        {"falso", FALSE},
        {"nese", IF},
        {"perndryshe", ELSE},
        {"kthen", RETURN}};

    TokenType LookupIdentifier(std::string_view identifier)
    {
        try
        {
            return keywords.at(identifier);
        }
        catch (const std::out_of_range &e)
        {
            return IDENT;
        }
    }
}
#include "UsernameCheck.h"

#include <cctype>

size_t UsernameCheck::MIN_LENGTH = 3;
size_t UsernameCheck::MAX_LENGTH = 16;

bool UsernameCheck::checkUsername(const std::string& username)
{
    if (username.length() < MIN_LENGTH || username.length() > MAX_LENGTH)
        return false;

    for (char c : username)
        if (c != '-' && c != '_'          //- and _
            && !isalnum(c))   //a-z, A-Z, numbers
            return false;

    return true;
}

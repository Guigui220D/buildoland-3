#include "UsernameCheck.h"

size_t UsernameCheck::MIN_LENGTH = 3;
size_t UsernameCheck::MAX_LENGTH = 16;

bool UsernameCheck::checkUsername(const std::string& username)
{
    if (username.length() < MIN_LENGTH || username.length() > MAX_LENGTH)
        return false;

    for (char c : username)
        if (c != 45 && c != 95          //- and _
            && !(c >= 97 && c <= 122)   //letters
            && !(c >= 65 && c <= 90)    //LETTERS
            && !(c >= 48 && c <= 57))   //numbers
            return false;

    return true;
}

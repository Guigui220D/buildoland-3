#pragma once

#include <string>

class UsernameCheck
{
    public:
        static size_t MIN_LENGTH;
        static size_t MAX_LENGTH;

        /**
        * @param username : the username to check
        * @return True if the username is valid
        */
        static bool checkUsername(const std::string& username);

    protected:

    private:
};

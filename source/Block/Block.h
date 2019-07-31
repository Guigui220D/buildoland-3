#pragma once

#include <string>

class Block
{
    public:
        Block(const std::string name);
        virtual ~Block();

        inline const std::string getName() const { return name; }

    private:
        std::string name;
};

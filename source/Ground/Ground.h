#pragma once

#include <string>

class Ground
{
    public:
        Ground(const std::string name);
        virtual ~Ground();

        inline const std::string getName() const { return name; }

    private:
        std::string name;
};

#pragma once

#include "../Ground.h"

class GroundError : public Ground
{
    public:
        GroundError();
        ~GroundError();

        bool acceptsTextureBleedings(GroundInfo info, const Ground* other) const;

    protected:

    private:
};


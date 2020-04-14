#pragma once

#include "../Ground.h"

class GroundCarpet : public Ground
{
public:
    GroundCarpet();
    ~GroundCarpet();

#ifdef CLIENT_SIDE
    inline uint8_t getTextureRotation(TileReference info) const { return 0; }
    inline bool hasTextureBleedings() const override { return true; }
    inline bool acceptsTextureBleedings(TileReference info, const Ground* other) const override { return false; }
    inline uint32_t getBleedingForNeighborGrounds(TileReference info, int frame) const override { return 6; }

    inline bool isNatural() const override { return false; }
#endif

protected:

private:
};


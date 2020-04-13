#pragma once

#include "../Ground.h"

class GroundCarpet : public Ground
{
public:
    GroundCarpet();
    ~GroundCarpet();

    inline bool isShovelable(TileReference info) const override { return false; }

#ifdef CLIENT_SIDE
    inline uint8_t getTextureRotation(TileReference info) const { 0; }
    inline bool hasTextureBleedings() const override { return true; }
    inline uint32_t getBleedingForNeighborGrounds(TileReference info, int frame) const override { return 6; }
#endif

protected:

private:
};


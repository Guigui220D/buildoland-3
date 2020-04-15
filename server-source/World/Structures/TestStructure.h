#include "../Structure.h"

class Block;
class Ground;

class TestStructure : public Structure
{
    public:
        TestStructure();
        ~TestStructure();

        const Block* getBlock(sf::Vector2i block_in_chunk) const override;
        const Ground* getGround(sf::Vector2i block_in_chunk) const override;

    protected:

    private:
};

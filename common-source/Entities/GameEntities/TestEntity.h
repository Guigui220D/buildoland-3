#pragma once

#include "../LivingEntity.h"

#ifdef CLIENT_SIDE
    #include "../../client-source/Utils/Animation.h"
#endif // CLIENT_SIDE

class TestEntity : public LivingEntity
{
    public:
        inline unsigned short getEntityCode() const { return Entities::TestEntity; };

        TestEntity(World* world, unsigned int id);
        ~TestEntity();

        void update(float delta);
        #ifdef CLIENT_SIDE
        void draw(sf::RenderTarget& target) const;

    private:
        sf::RectangleShape rs;
        sf::CircleShape shadow;
        #endif
};
#pragma once

#include "Inventory.h"

#ifdef CLIENT_SIDE
    #include "../Networking/ServerToClientRequests.h"
#endif

class Player;

class PlayerInventory : public Inventory<25u>
{
    public:
        PlayerInventory(const Player& owner);

        void swapHands(int pos);
        void dropHand();

        inline ItemStack& getHand() { return getStack(0); }

        #ifdef CLIENT_SIDE
        bool handleInventoryUpdateRequest(const Networking::StoC::InventoryUpdateRequest &rq);
        #endif // CLIENT_SIDE

        void onInsert(const ItemStack& stack) override;

    protected:

    private:
        bool preparing = true;

        const Player& owner;
};

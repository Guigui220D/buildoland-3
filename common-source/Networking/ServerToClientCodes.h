#pragma once

#include <stdint.h>

namespace Networking
{
    namespace StoC
    {
        enum
        {
            Disconnect = 0,

            ConnectionAccepted,
            ConnectionRefused,
            FinalHandshake,
            SendChunk,
            EntityAction,
            TileEntityUpdate,
            BlockUpdate,
            GroundUpdate,
            PlayerRectification,
            InventoryUpdate
        };

        enum ConnectionRefusalReason : uint8_t
        {
            InvalidNickname,
            NicknameAlreadyChosen
        };
    }
}


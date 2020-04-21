#pragma once

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
    }
}


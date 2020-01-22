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
            BlockUpdate,
            GroundUpdate,
            PlayerRectification,
            InventoryUpdate
        };
    }
}

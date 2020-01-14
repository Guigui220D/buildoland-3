#pragma once

namespace Networking
{
    namespace StoC
    {
        enum : unsigned short
        {
            Disconnect = 0,
            ConnectionAccepted,
            ConnectionRefused,
            FinalHandshake,
            SendChunk,
            EntityAction,
            BlockUpdate,
            GroundUpdate,
            PlayerRectification
        };
    }
}

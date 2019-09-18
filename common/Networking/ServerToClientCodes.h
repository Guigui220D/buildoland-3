#pragma once

namespace Networking
{
    namespace StoC
    {
        enum : unsigned short
        {
            Disconnect = 0,
            SoloHandshake,
            ConnexionAccepted,
            ConnexionRefused,
            SendChunk
        };
    }
}
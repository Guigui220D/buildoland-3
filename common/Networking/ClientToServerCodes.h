#pragma once

namespace Networking
{
    namespace CtoS
    {
        enum : unsigned short
        {
            Disconnect = 0,
            AskForConnection,
			RequestChunk
        };
    }
}

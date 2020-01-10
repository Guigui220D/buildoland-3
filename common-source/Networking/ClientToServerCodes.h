#pragma once

namespace Networking
{
    namespace CtoS
    {
        enum : unsigned short
        {
            Disconnect = 1,
            RequestConnection,
			RequestChunk,
			RequestEntityInfo,
			PlayerAction
        };
    }
}

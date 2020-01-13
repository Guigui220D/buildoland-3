#pragma once

namespace Networking
{
    namespace CtoS
    {
        enum : unsigned short
        {
            KeepAlive = 0,
            Disconnect,
            RequestConnection,
			RequestChunk,
			RequestEntityInfo,
			PlayerAction
        };
    }
}

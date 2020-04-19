#pragma once

namespace Networking
{
    namespace CtoS
    {
        enum
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

#pragma once

#include <cstdint>

namespace utils
{
    constexpr int64_t combine(int32_t a, int32_t b)
    {
        return (((int64_t)a << 32) | b);
    }
}

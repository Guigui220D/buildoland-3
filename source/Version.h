#pragma once

namespace Version
{
    const char VERSION[] =          "Infdev 3.0.5";

    const char VERSION_SHORT[] =    "3inf5"; //Should be 5 char long
    static_assert(sizeof(VERSION_SHORT) == 6);
}

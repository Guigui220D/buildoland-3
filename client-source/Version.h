#pragma once

namespace Version
{
    const char VERSION[] =          "Indev 3.0.E";

    const char VERSION_SHORT[] =    "3indE"; //Should be 5 char long
    static_assert(sizeof(VERSION_SHORT) == 6, "what");
}

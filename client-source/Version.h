#pragma once

namespace Version
{
    const char VERSION[] =          "Indev 3.0.4";

    const char VERSION_SHORT[] =    "3ind4"; //Should be 5 char long
    static_assert(sizeof(VERSION_SHORT) == 6);
}

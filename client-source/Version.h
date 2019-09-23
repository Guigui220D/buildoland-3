#pragma once

namespace Version
{
    const char VERSION[] =          "Indev 3.0.3";

    const char VERSION_SHORT[] =    "3ind3"; //Should be 5 char long
    static_assert(sizeof(VERSION_SHORT) == 6);
}

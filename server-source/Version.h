#pragma once

namespace Version
{
    const char VERSION[] =          "Indev 3.0.6";

    const char VERSION_SHORT[] =    "3ind6"; //Should be 5 char long
    static_assert(sizeof(VERSION_SHORT) == 6);
}

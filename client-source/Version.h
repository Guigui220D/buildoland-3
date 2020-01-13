#pragma once

namespace Version
{
    const char VERSION[] =          "Pre Alpha 3";

    const char VERSION_SHORT[] =    "3preA"; //Should be 5 char long
    static_assert(sizeof(VERSION_SHORT) == 6);
}

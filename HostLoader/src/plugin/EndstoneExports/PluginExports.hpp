#pragma once

#include "Export.hpp"

extern "C"
{
    EXPORT const endstone::Logger* plugin_getLogger(endstone::Plugin* plugin)
    {
        return &plugin->getLogger();
    }
}
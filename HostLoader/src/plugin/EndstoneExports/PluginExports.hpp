#pragma once

#include "Export.hpp"

extern "C"
{
    EXPORT const endstone::Logger* plugin_getLogger(endstone::Plugin* plugin)
    {
        return &plugin->getLogger();
    }

    EXPORT const char* plugin_getName(endstone::Plugin* plugin)
    {
        static thread_local std::string temp;
        temp = std::string(plugin->getName()); 
        return temp.c_str();
    }

    EXPORT endstone::Server* plugin_getServer(endstone::Plugin* plugin)
    {
        return &plugin->getServer();
    }
}
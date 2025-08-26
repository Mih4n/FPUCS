#pragma once

extern "C"
{
    EXPORT const char* server_getName(endstone::Server* server) {
        static thread_local std::string temp;
        temp = std::string(server->getName()); 
        return temp.c_str();
    }

    EXPORT const char* server_getVersion(endstone::Server* server) {
        static thread_local std::string temp;
        temp = std::string(server->getVersion()); 
        return temp.c_str();
    }

    EXPORT const char* server_getMinecraftVersion(endstone::Server* server) {
        static thread_local std::string temp;
        temp = std::string(server->getMinecraftVersion()); 
        return temp.c_str();
    }

    EXPORT int server_getProtocolVersion(endstone::Server* server) {
        return server->getProtocolVersion();
    }

    EXPORT endstone::Logger* server_getLogger(endstone::Server* server) {
        return &server->getLogger();
    }

    // todo
}
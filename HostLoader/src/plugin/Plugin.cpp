#include <iostream>

#include "Plugin.h"

namespace HostLoader {
    Plugin::PluginInfo::PluginInfo() {
        name        = "my_plugin";
        version     = "11.45.14";
        prefix      = "CppExamplePlugin";
        description = "C++ Example Plugin";
        website     = "https://github.com/Owner/Repo";
        authors     = {"Me"};
    }

    void Plugin::onLoad() {
        if (!TryLoadHostEntry(&mEntryPoint, mHostContext)) return; 

        mPluginEntry.pluginPointer = this;
        mEntryPoint(&mPluginEntry, sizeof(mPluginEntry));

        mPluginEntry.OnLoad();
        getLogger().debug("Loading... C++");
    }

    void Plugin::onEnable() {
        mPluginEntry.OnEnable();
        getLogger().debug("Enabling... C++");
    }

    void Plugin::onDisable() {
        mPluginEntry.OnDisable();
        getLogger().debug("Disabling... C++");

        mHostContext.CloseFn(mHostContext.Handle);
    }
} // namespace HostLoader

extern "C" [[maybe_unused]] ENDSTONE_EXPORT endstone::Plugin* init_endstone_plugin() {
    return HostLoader::Plugin::getInstance();
}
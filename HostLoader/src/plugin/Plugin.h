#pragma once

#include "Dotnet/HostLoader.hpp"
#include "EndstoneExports/Exports.hpp"

namespace HostLoader {

class Plugin : public endstone::Plugin {
public:
    struct PluginInfo : public endstone::detail::PluginDescriptionBuilder {
        std::string name;
        std::string version;
        PluginInfo();
    };

public:
    static Plugin* getInstance() {
        static auto instance = new Plugin();
        return instance;
    }

    const endstone::PluginDescription& getDescription() const override {
        static auto description = PluginInfo().build(mPluginInfo.name, mPluginInfo.version);
        return description;
    }

    void onLoad() override;

    void onEnable() override;

    void onDisable() override;

private:
    HostContext mHostContext;
    PluginInfo mPluginInfo;
    PluginEntry mPluginEntry;
    EntryPointFunction mEntryPoint;
};

} // namespace my_plugin
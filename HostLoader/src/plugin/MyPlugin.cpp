#include "MyPlugin.h"

namespace my_plugin {

MyPlugin::PluginInfo::PluginInfo() {
    name        = "my_plugin";
    version     = "11.45.14";
    prefix      = "CppExamplePlugin";
    description = "C++ Example Plugin";
    website     = "https://github.com/Owner/Repo";
    authors     = {"Me"};
}

void MyPlugin::onLoad() {
    getLogger().debug("Loading...");
    // Code for loading the mod goes here.
}

void MyPlugin::onEnable() {
    getLogger().debug("Enabling...");
    // Code for enabling the mod goes here.
}

void MyPlugin::onDisable() {
    getLogger().debug("Disabling...");
    // Code for disabling the mod goes here.
}

} // namespace my_plugin

extern "C" [[maybe_unused]] ENDSTONE_EXPORT endstone::Plugin* init_endstone_plugin() {
    return my_plugin::MyPlugin::getInstance();
}
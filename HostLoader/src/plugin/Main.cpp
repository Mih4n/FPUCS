// #include <iostream>

// #include "Dotnet/HostLoader.hpp"

// int main()
// {
//     HostContext ctx;
//     EntryPointFunction entry = nullptr;
//     if (!TryLoadHostEntry(&entry, ctx)) return 1;   
    
//     PluginEntry* plugin = new PluginEntry();

//     plugin->OnLoad = []() {
//         std::cout << "Loaded" << std::endl;
//     };

//     plugin->OnEnable = []() {
//         std::cout << "Enabled" << std::endl;
//     };

//     plugin->OnDisable = []() {
//         std::cout << "Disabled" << std::endl;
//     };

//     std::cout << "Hello World" << std::endl;

//     entry(plugin, 1);

//     std::cout << "Goodbye World" << std::endl;    

//     plugin->OnLoad();
//     plugin->OnEnable();
//     plugin->OnDisable();

//     ctx.CloseFn(ctx.Handle);
//     return 0;
// }
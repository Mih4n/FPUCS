using DotnetLoaderLibrary.Base.Logger;
using DotnetLoaderLibrary.Plugin;

namespace TestPlugin;

public class Plugin : PluginBase
{
    public override void OnLoad() 
    {
        // Logger?.Info($"Plugin: {Name}");

        // Logger?.Info($"Server version: {Server?.Version}");
        // Logger?.Info($"Server name: {Server?.Name}");
        // Logger?.Info($"Server port: {Server?.MinecraftVersion}");
        Logger?.Info($"Server protocol: {Server?.ProtocolVersion}");

        Server?.Logger?.Info("That is a message from the server logger!");

        Logger?.Info("Plugin loaded!");
    }

    public override void OnEnable()
    {
        Logger?.Info("Plugin enabled!");
    }

    public override void OnDisable()
    {
        Logger?.Log(LogLevel.Trace, "Plugin disabled!");
        Logger?.Info("Plugin disabled!");
        Logger?.Warning("Plugin disabled!");
        Logger?.Error("Plugin disabled!");
        Logger?.Critical("Plugin disabled!");
    }
}

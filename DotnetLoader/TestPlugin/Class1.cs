using DotnetLoaderLibrary;

namespace TestPlugin;

public class Plugin : PluginBase
{
    public override void OnLoad() 
    {
        if (Logger == null) return;
        Logger.Info("Plugin loaded!");
    }

    public override void OnEnable()
    {
        if (Logger == null) return;
        Logger.Info("Plugin enabled!");
    }

    public override void OnDisable()
    {
        if (Logger == null) return;
        Logger.Info("Plugin disabled!");
    }
}

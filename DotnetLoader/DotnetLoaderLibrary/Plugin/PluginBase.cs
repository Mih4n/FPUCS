using DotnetLoaderLibrary.Base.Logger;
using DotnetLoaderLibrary.Base.Server;

namespace DotnetLoaderLibrary.Plugin;

public class PluginBase : IPlugin
{
    public nint Pointer { get; set; } = nint.Zero;
    public string Name => Pointer == nint.Zero ? string.Empty : PluginImports.GetName(Pointer); 

    public IServer? Server => 
        Pointer == nint.Zero ? 
            null : 
            new ServerModel(PluginImports.GetServer(Pointer));
    public ILogger? Logger => 
        Pointer == nint.Zero ? 
            null : 
            new LoggerModel(PluginImports.GetLogger(Pointer));

    public virtual void OnLoad() { }
    public virtual void OnEnable() { }
    public virtual void OnDisable() { }
}

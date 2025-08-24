namespace DotnetLoaderLibrary;

public class PluginBase : IPlugin
{
    public ILogger? Logger { get; set; }

    public virtual void OnLoad() { }
    public virtual void OnEnable() { }
    public virtual void OnDisable() { }
}

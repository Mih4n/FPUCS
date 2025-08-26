using DotnetLoaderLibrary.Base.Logger;
using DotnetLoaderLibrary.Base.Server;

namespace DotnetLoaderLibrary.Plugin;

public interface IPlugin
{
    string Name { get; }
    IServer? Server { get; }
    ILogger? Logger { get; }

    void OnLoad();
    void OnEnable();
    void OnDisable();
}

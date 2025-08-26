using DotnetLoaderLibrary.Base.Logger;

namespace DotnetLoaderLibrary;

public interface IPlugin
{
    ILogger? Logger { set; get; }

    void OnLoad();
    void OnEnable();
    void OnDisable();
}

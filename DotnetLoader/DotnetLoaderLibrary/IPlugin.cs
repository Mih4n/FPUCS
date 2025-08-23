namespace DotnetLoaderLibrary;

public interface IPlugin
{
    bool Enabled { get; }
    ILogger Logger { get; }

    void OnLoad();
    void OnEnable();
    void OnDisable();
}

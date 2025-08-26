using DotnetLoaderLibrary.Base.Logger;

namespace DotnetLoaderLibrary.Base.Server;

public interface IServer
{
    public int ProtocolVersion { get; }
    public string Name { get; }
    public string Version { get; }
    public string MinecraftVersion { get; }
    public ILogger Logger { get; }
}

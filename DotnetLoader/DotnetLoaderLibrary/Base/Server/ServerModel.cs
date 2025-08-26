using DotnetLoaderLibrary.Base.Logger;

namespace DotnetLoaderLibrary.Base.Server;

public class ServerModel(nint pointer) : IServer
{
    public int ProtocolVersion => ServerImports.GetProtocolVersion(pointer);
    public string Name => ServerImports.GetName(pointer);
    public string Version => ServerImports.GetVersion(pointer);
    public string MinecraftVersion => ServerImports.GetMinecraftVersion(pointer);
    public ILogger Logger => new LoggerModel(ServerImports.GetLogger(pointer));
}

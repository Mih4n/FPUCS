using System.Runtime.InteropServices;

namespace DotnetLoaderLibrary.Base.Server;

public static class ServerImports
{
    [return: MarshalAs(UnmanagedType.LPUTF8Str)]
    [DllImport(Constants.HostLoaderLibrary, CallingConvention = CallingConvention.Cdecl, EntryPoint = "server_getName")]
    public static extern string GetName(nint pointer);

    [return: MarshalAs(UnmanagedType.LPUTF8Str)]
    [DllImport(Constants.HostLoaderLibrary, CallingConvention = CallingConvention.Cdecl, EntryPoint = "server_getVersion")]
    public static extern string GetVersion(nint pointer);

    [return: MarshalAs(UnmanagedType.LPUTF8Str)]
    [DllImport(Constants.HostLoaderLibrary, CallingConvention = CallingConvention.Cdecl, EntryPoint = "server_getMinecraftVersion")]
    public static extern string GetMinecraftVersion(nint pointer);

    [DllImport(Constants.HostLoaderLibrary, CallingConvention = CallingConvention.Cdecl, EntryPoint = "server_getProtocolVersion")]
    public static extern int GetProtocolVersion(nint pointer);

    [DllImport(Constants.HostLoaderLibrary, CallingConvention = CallingConvention.Cdecl, EntryPoint = "server_getLogger")]
    public static extern nint GetLogger(nint pointer);
}

using System.Runtime.InteropServices;

namespace DotnetLoaderLibrary.Plugin;

public static class PluginImports
{
    [DllImport(Constants.HostLoaderLibrary, CallingConvention = CallingConvention.Cdecl, EntryPoint = "plugin_getLogger")]
    public static extern nint GetLogger(nint pointer);

    [return: MarshalAs(UnmanagedType.LPUTF8Str)]
    [DllImport(Constants.HostLoaderLibrary, CallingConvention = CallingConvention.Cdecl, EntryPoint = "plugin_getName")]
    public static extern string GetName(nint pointer);

    [DllImport(Constants.HostLoaderLibrary, CallingConvention = CallingConvention.Cdecl, EntryPoint = "plugin_getServer")]
    public static extern nint GetServer(nint pointer);
}

using System.Runtime.InteropServices;

namespace DotnetLoader.Plugin;

public static class PluginImports
{
    [DllImport(Constants.HostLoaderLibrary, CallingConvention = CallingConvention.Cdecl, EntryPoint = "plugin_getLogger")]
    public static extern nint GetLogger(nint pointer);
}

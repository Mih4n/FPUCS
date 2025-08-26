using System.Runtime.InteropServices;

namespace DotnetLoader;

public static class Entry
{
    private static PluginManager pluginManager = new();

    public static int Initialize(nint arg, int _)
    {
        var callbacks = Marshal.PtrToStructure<PluginCallbacks>(arg);
        pluginManager.LoadPlugins(ref callbacks);
        pluginManager.RegisterCallbacks(ref callbacks);
        Marshal.StructureToPtr(callbacks, arg, true);

        return 0;
    }
}
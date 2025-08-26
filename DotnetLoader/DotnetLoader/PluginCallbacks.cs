using System.Runtime.InteropServices;

namespace DotnetLoader;

[StructLayout(LayoutKind.Sequential)]
public struct PluginCallbacks
{
    /// <summary>
    /// represents a pointer to the plugin
    /// </summary>
    public nint Pointer;

    public nint OnLoad;
    public nint OnEnable;
    public nint OnDisable;
}

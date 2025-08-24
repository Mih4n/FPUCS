using System.Runtime.InteropServices;

namespace DotnetLoader;

[StructLayout(LayoutKind.Sequential)]
public struct PluginCallbacks
{
    public nint OnLoad;
    public nint OnEnable;
    public nint OnDisable;
}

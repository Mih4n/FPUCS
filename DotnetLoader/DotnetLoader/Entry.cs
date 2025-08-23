using System.Runtime.InteropServices;
using DotnetLoaderLibrary;

namespace DotnetLoader;

public static class Entry 
{
    [UnmanagedCallersOnly]
    public static void OnLoad()
    {
        if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
        {
            Console.WriteLine("Windows");
        }
        else if (RuntimeInformation.IsOSPlatform(OSPlatform.Linux))
        {
            Console.WriteLine("Linux");
        }
        else if (RuntimeInformation.IsOSPlatform(OSPlatform.OSX))
        {
            Console.WriteLine("Mac");
        }

        Console.WriteLine("C# OnLoad called!");
    }

    [UnmanagedCallersOnly]
    public static void OnEnable()
    {
        Console.WriteLine("C# OnEnable called!");
    }

    [UnmanagedCallersOnly]
    public static void OnDisable()
    {
        Console.WriteLine("C# OnDisable called!");
    }

    public static int Initialize(nint arg, int size)
    {
        PluginInfo info = Marshal.PtrToStructure<PluginInfo>(arg);

        Action onLoad = Marshal.GetDelegateForFunctionPointer<Action>(info.OnLoad);
        Action onEnable = Marshal.GetDelegateForFunctionPointer<Action>(info.OnEnable);
        Action onDisable = Marshal.GetDelegateForFunctionPointer<Action>(info.OnDisable);

        onLoad();
        onEnable();
        onDisable();

        unsafe 
        {
            Plugin* plugin = (Plugin*)arg;
            plugin->OnLoad = &OnLoad;
            plugin->OnEnable = &OnEnable;
            plugin->OnDisable = &OnDisable;
        }

        Console.WriteLine("Hello from C#");
        return 0;
    }
}

[StructLayout(LayoutKind.Sequential)]
unsafe struct Plugin
{
    public delegate* unmanaged<void> OnLoad;
    public delegate* unmanaged<void> OnEnable;
    public delegate* unmanaged<void> OnDisable;
}

[StructLayout(LayoutKind.Sequential)]
public struct PluginInfo
{
    public nint OnLoad;
    public nint OnEnable;
    public nint OnDisable;
}
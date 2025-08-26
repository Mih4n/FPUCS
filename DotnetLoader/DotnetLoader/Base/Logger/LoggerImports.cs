using System.Runtime.InteropServices;
using DotnetLoaderLibrary.Base.Logger;

namespace DotnetLoader.Base.Logger;

public static class LoggerImports 
{
    [return: MarshalAs(UnmanagedType.LPUTF8Str)]
    [DllImport(Constants.HostLoaderLibrary, CallingConvention = CallingConvention.Cdecl, EntryPoint = "logger_getName")]
    public static extern string GetName(nint pointer);

    [DllImport(Constants.HostLoaderLibrary, CallingConvention = CallingConvention.Cdecl, EntryPoint = "logger_setLevel")]
    public static extern void SetLevel(nint pointer);

    [DllImport(Constants.HostLoaderLibrary, CallingConvention = CallingConvention.Cdecl, EntryPoint = "logger_isEnabledFor")]
    public static extern bool IsEnabledFor(nint pointer, int level);
    
    [DllImport(Constants.HostLoaderLibrary, CallingConvention = CallingConvention.Cdecl, EntryPoint = "logger_log")]
    public static extern void Log(nint pointer, LogLevel level, [MarshalAs(UnmanagedType.LPUTF8Str)] string message); 
}
using System.Runtime.InteropServices;

namespace DotnetLoader;

public class Entry 
{
    [UnmanagedCallersOnly(EntryPoint = "HelloFromDotnet")]
    public static void Hello() => Console.WriteLine("Привет из .NET!");
}
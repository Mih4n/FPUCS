using System;

namespace DotnetLoader
{
    public static class Entry 
    {
        public static int Hello(IntPtr arg, int argLength)
        {
            Console.WriteLine("Hello from C#");
            return 0;
        }
    }
}
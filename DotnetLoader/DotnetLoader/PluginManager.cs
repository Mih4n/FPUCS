using System.Reflection;
using System.Runtime.InteropServices;
using DotnetLoaderLibrary.Base.Logger;
using DotnetLoaderLibrary.Plugin;

namespace DotnetLoader;

public class PluginManager
{
    private readonly List<IPlugin> plugins = [];
    
    private Action onLoad = () => { };
    private Action onEnable = () => { };
    private Action onDisable = () => { };

    public void LoadPlugins(ref PluginCallbacks callbacks)
    {
        AppDomain.CurrentDomain.AssemblyResolve += (sender, args) =>
        {
            var name = new AssemblyName(args.Name).Name + ".dll";
            var path = Path.Combine("./plugins/dotnet/loader", name);
            if (File.Exists(path))
                return Assembly.LoadFrom(path);
            return null;
        };

        string[] dlls = Directory.GetFiles(Path.Combine(Directory.GetCurrentDirectory(), "plugins/dotnet"), "*.dll");
        foreach (string dll in dlls)
        {
            var loadContext = new PluginLoadContext(dll);
            Assembly assembly = loadContext.LoadFromAssemblyPath(dll);
            
            foreach (Type type in assembly.GetTypes())
            {
                if (!type.IsSubclassOf(typeof(PluginBase))) continue;

                Console.WriteLine($"Found plugin: {type.Name}");

                if (Activator.CreateInstance(type) is PluginBase plugin)
                {
                    plugin.Pointer = callbacks.Pointer;

                    plugins.Add(plugin);

                    onLoad += plugin.OnLoad;
                    onEnable += plugin.OnEnable;
                    onDisable += plugin.OnDisable;
                }
            }
        }
    }

    public void RegisterCallbacks(ref PluginCallbacks callbacks)
    {
        Console.WriteLine("Registering callbacks...");

        callbacks.OnLoad = Marshal.GetFunctionPointerForDelegate(onLoad);
        callbacks.OnEnable = Marshal.GetFunctionPointerForDelegate(onEnable);
        callbacks.OnDisable = Marshal.GetFunctionPointerForDelegate(onDisable);
    }
}
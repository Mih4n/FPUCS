using System.Reflection;
using System.Runtime.InteropServices;
using DotnetLoaderLibrary;

namespace DotnetLoader;

public class PluginManager
{
    private readonly List<IPlugin> plugins = [];
    
    private Action onLoad = () => { };
    private Action onEnable = () => { };
    private Action onDisable = () => { };

    public void LoadPlugins()
    {
        string[] dlls = Directory.GetFiles(Directory.GetCurrentDirectory(), "*.dll");
        foreach (string dll in dlls)
        {
            Assembly assembly = Assembly.LoadFile(dll);
            foreach (Type type in assembly.GetTypes())
            {
                if (!type.IsSubclassOf(typeof(PluginBase)))
                    continue;

                if (Activator.CreateInstance(type) is PluginBase plugin)
                {
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
        callbacks.OnLoad = Marshal.GetFunctionPointerForDelegate(onLoad);
        callbacks.OnEnable = Marshal.GetFunctionPointerForDelegate(onEnable);
        callbacks.OnDisable = Marshal.GetFunctionPointerForDelegate(onDisable);
    }
}

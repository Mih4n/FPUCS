using System.Reflection;
using System.Runtime.Loader;

namespace DotnetLoader;

public class PluginLoadContext : AssemblyLoadContext
{
    private readonly string pluginPath;

    public PluginLoadContext(string pluginPath) : base(isCollectible: true)
    {
        this.pluginPath = pluginPath;
    }

    protected override Assembly? Load(AssemblyName assemblyName)
    {
        if (assemblyName.Name == "DotnetLoaderLibrary")
        {
            return Assembly.Load(assemblyName);
        }

        string path = Path.Combine(Path.GetDirectoryName(pluginPath)!, assemblyName.Name + ".dll");
        if (File.Exists(path))
        {
            return LoadFromAssemblyPath(path);
        }

        return null;
    }
}

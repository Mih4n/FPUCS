using DotnetLoaderLibrary.Base.Logger;

namespace DotnetLoader.Base.Logger;

public class Logger(nint pointer) : ILogger
{
    public string Name => LoggerImports.GetName(pointer);

    public void SetLevel(LogLevel level) { }

    public bool IsEnabledFor(LogLevel level) => true;

    public void Log(LogLevel level, string message) 
        => LoggerImports.Log(pointer, level, message);

    public void Info(string message) 
        => LoggerImports.Log(pointer, LogLevel.Info, message); 

    public void Trace(string message) 
        => LoggerImports.Log(pointer, LogLevel.Trace, message);

    public void Error(string message)
        => LoggerImports.Log(pointer, LogLevel.Error, message);

    public void Warning(string message)
        => LoggerImports.Log(pointer, LogLevel.Warning, message);

    public void Critical(string message) 
        => LoggerImports.Log(pointer, LogLevel.Critical, message); 
}

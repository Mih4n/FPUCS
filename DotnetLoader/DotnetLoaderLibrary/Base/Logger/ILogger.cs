namespace DotnetLoaderLibrary.Base.Logger;

public interface ILogger
{
    string Name { get; }

    void SetLevel(LogLevel level);
    bool IsEnabledFor(LogLevel level);

    void Log(LogLevel level, string message);

    void Info(string message);
    void Trace(string message);
    void Error(string message);
    void Warning(string message);
    void Critical(string message);
}

#include <iostream>
#include <string>
#include <vector>
#include <nethost.h>
#include <hostfxr.h>
#include <coreclr_delegates.h>

#ifdef _WIN32
#include <Windows.h>
#include <io.h>
#define STR(s) L ## s
#define DIR_SEPARATOR L'\\'
using char_t = wchar_t;
using LibHandle = HMODULE;
inline LibHandle LoadLibrary(const char_t* path) { return ::LoadLibraryW(path); }
inline void* GetExport(LibHandle lib, const char* name) { return (void*)::GetProcAddress(lib, name); }
#else
#include <unistd.h>
#include <dlfcn.h>
#include <limits.h>
#define STR(s) s
#define DIR_SEPARATOR '/'
#define MAX_PATH PATH_MAX
using char_t = char;
using LibHandle = void*;
inline LibHandle LoadLibrary(const char_t* path) { return dlopen(path, RTLD_LAZY | RTLD_LOCAL); }
inline void* GetExport(LibHandle lib, const char* name) { return dlsym(lib, name); }
#endif

struct Plugin
{
    void (*OnLoad)() { nullptr };
    void (*OnEnable)() { nullptr };
    void (*OnDisable)() { nullptr };
};

using EntryPointFunction = int(*)(void* arg, int argLength);

struct HostContext {
    LibHandle Library { nullptr };
    hostfxr_handle Handle { nullptr };
    hostfxr_close_fn CloseFn { nullptr };
    hostfxr_get_runtime_delegate_fn GetDelegateFn { nullptr };
    hostfxr_initialize_for_runtime_config_fn InitConfigFn { nullptr };
    load_assembly_and_get_function_pointer_fn LoadAssemblyFn { nullptr };
};

bool TryGetExecutablePath(std::basic_string<char_t>& path)
{
#ifdef _WIN32
    std::vector<char_t> buf(MAX_PATH);
    DWORD len = GetModuleFileNameW(nullptr, buf.data(), (DWORD)buf.size());
    if (len == 0) return false;
    path.assign(buf.data(), len);
#else
    char_t buf[MAX_PATH];
    ssize_t len = readlink("/proc/self/exe", buf, sizeof(buf));
    if (len == -1 || len == sizeof(buf)) return false;
    buf[len] = '\0';
    path.assign(buf);
#endif
    size_t lastSlash = path.find_last_of(DIR_SEPARATOR);
    if (lastSlash != std::basic_string<char_t>::npos)
        path.resize(lastSlash + 1);
    return true;
}

bool TryGetHostPath(std::basic_string<char_t>& hostPath)
{
    char_t buffer[MAX_PATH];
    size_t len = sizeof(buffer) / sizeof(buffer[0]);
    if (get_hostfxr_path(buffer, &len, nullptr) != 0)
        return false;
    hostPath.assign(buffer, len);
    return true;
}

bool TryInitializeHostFxr(const std::basic_string<char_t>& hostPath, HostContext& ctx)
{
    ctx.Library = LoadLibrary(hostPath.c_str());
    if (!ctx.Library) return false;

    ctx.CloseFn = (hostfxr_close_fn)GetExport(ctx.Library, "hostfxr_close");
    ctx.InitConfigFn = (hostfxr_initialize_for_runtime_config_fn)GetExport(ctx.Library, "hostfxr_initialize_for_runtime_config");
    ctx.GetDelegateFn = (hostfxr_get_runtime_delegate_fn)GetExport(ctx.Library, "hostfxr_get_runtime_delegate");

    return ctx.CloseFn && ctx.GetDelegateFn && ctx.InitConfigFn;
}

bool TryLoadManagedMethod(const std::basic_string<char_t>& path, HostContext& ctx, EntryPointFunction* entry)
{
    std::basic_string<char_t> configPath = path + STR("runtime.config.json");
    std::basic_string<char_t> assemblyPath = path + STR("DotnetLoader.dll");

    if (ctx.InitConfigFn(configPath.c_str(), nullptr, &ctx.Handle) != 0 || !ctx.Handle)
        return false;

    if (ctx.GetDelegateFn(ctx.Handle, hdt_load_assembly_and_get_function_pointer, (void**)&ctx.LoadAssemblyFn) != 0)
        return false;

    const char_t* dotnetType = STR("DotnetLoader.Entry, DotnetLoader");
    const char_t* dotnetMethod = STR("Initialize");

    int result = ctx.LoadAssemblyFn(
        assemblyPath.c_str(),
        dotnetType,
        dotnetMethod,
        nullptr,
        nullptr,
        (void**)entry
    );
    return (result == 0 && *entry);
}

int main()
{
    HostContext ctx;
    std::basic_string<char_t> hostPath;
    if (!TryGetHostPath(hostPath) || !TryInitializeHostFxr(hostPath, ctx)) {
        std::cerr << "Failed to initialize hostfxr" << std::endl;
        return 1;
    }

    std::basic_string<char_t> configPath;
    if (!TryGetExecutablePath(configPath)) {
        std::cerr << "Failed to get executable path" << std::endl;
        return 1;
    }

    EntryPointFunction entry = nullptr;
    if (!TryLoadManagedMethod(configPath, ctx, &entry)) {
        std::cerr << "Failed to load managed Hello function" << std::endl;
        return 1;
    }

    Plugin* plugin = new Plugin();

    plugin->OnLoad = []() {
        std::cout << "Hello from C++ plugin" << std::endl;
    };

    plugin->OnEnable = []() {
        std::cout << "Enabled" << std::endl;
    };

    plugin->OnDisable = []() {
        std::cout << "Disabled" << std::endl;
    };

    entry(plugin, 1);

    plugin->OnLoad();
    plugin->OnEnable();
    plugin->OnDisable();

    ctx.CloseFn(ctx.Handle);
    return 0;
}
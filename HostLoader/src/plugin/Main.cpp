#include <iostream>
#include <string>
#include <string>
#include <vector>
#include <nethost.h>
#include <hostfxr.h>
#include <coreclr_delegates.h>

#ifdef _WIN32
#include <Windows.h>
#define STR(s) L ## s
#define CH(c) L ## c
#define DIR_SEPARATOR L'\\'
#define StringCompare wcscmp
using LibHandle = HMODULE;
inline LibHandle LoadLibrary(const char_t* path) { return ::LoadLibraryW(path); }
inline void* GetExport(LibHandle lib, const char* name) { return (void*)::GetProcAddress(lib, name); }

#else
#include <dlfcn.h>
#include <limits.h>
#define STR(s) s
#define CH(c) c
#define DIR_SEPARATOR '/'
#define MAX_PATH PATH_MAX
#define StringCompare strcmp
using LibHandle = void*;
inline LibHandle LoadLibrary(const char_t* path) { return dlopen(path, RTLD_LAZY | RTLD_LOCAL); }
inline void* GetExport(LibHandle lib, const char* name) { return dlsym(lib, name); }
#endif

using CloseHost = hostfxr_close_fn;
using HostHandler = hostfxr_handle;
using GetRuntimeDelegate = hostfxr_get_runtime_delegate_fn;
using InitializeForRuntimeConfig = hostfxr_initialize_for_runtime_config_fn;
using LoadAssemblyAndGetFunctionPtr = load_assembly_and_get_function_pointer_fn;
using HelloFunction = void(void);

char_t HostPath[MAX_PATH];
LibHandle HostfxrLibrary = nullptr;
HostHandler HostFxrHandle = nullptr;
CloseHost CloseHostFn = nullptr;
GetRuntimeDelegate GetDelegateFn = nullptr;
InitializeForRuntimeConfig InitForConfigFn = nullptr;
LoadAssemblyAndGetFunctionPtr LoadAssemblyFn = nullptr;
HelloFunction* HelloFromDotnet = nullptr;

bool TryGetExecutablePath(std::basic_string<char_t>& path)
{
#ifdef _WIN32
    std::vector<char_t> buf(MAX_PATH);
    DWORD len = GetModuleFileNameW(nullptr, buf.data(), (DWORD)buf.size());
    if (len == 0) return false;
    path = std::basic_string<char_t>(buf.data(), len);
    size_t last_slash = path.find_last_of(DIR_SEPARATOR);
    if (last_slash != std::basic_string<char_t>::npos) {
        path.resize(last_slash + 1);
    }
    return true;
#else
    char_t buf[MAX_PATH];
    ssize_t len = readlink("/proc/self/exe", buf, sizeof(buf));
    if (len == -1 || len == sizeof(buf)) return false;
    buf[len] = '\0';
    path = std::basic_string<char_t>(buf);
    size_t last_slash = path.find_last_of(DIR_SEPARATOR);
    if (last_slash != std::basic_string<char_t>::npos) {
        path.resize(last_slash + 1);
    }
    return true;
#endif
}

bool TryGetHostPath()
{
    size_t len = sizeof(HostPath) / sizeof(HostPath[0]);
    return get_hostfxr_path(HostPath, &len, nullptr) == 0;
}

bool InitializeHostFxr()
{
    if (!TryGetHostPath()) return false;
    HostfxrLibrary = LoadLibrary(HostPath);
    if (!HostfxrLibrary) return false;
    return true;
}

bool GetDelegates()
{
    CloseHostFn = (CloseHost)GetExport(HostfxrLibrary, "hostfxr_close");
    GetDelegateFn = (GetRuntimeDelegate)GetExport(HostfxrLibrary, "hostfxr_get_runtime_delegate");
    InitForConfigFn = (InitializeForRuntimeConfig)GetExport(HostfxrLibrary, "hostfxr_initialize_for_runtime_config");
    return CloseHostFn && GetDelegateFn && InitForConfigFn;
}

bool LoadManagedMethod(const char_t* configPath)
{
    wprintf(L"Loading runtime config: %ls\n", configPath);
    if (_waccess(configPath, 0) != 0) {
        std::cerr << "runtime.config.json does not exist or is inaccessible" << std::endl;
        return false;
    }

    int result = InitForConfigFn(configPath, nullptr, &HostFxrHandle);
    if (result != 0 || !HostFxrHandle)
    {
        std::cerr << "Failed to initialize hostfxr with result: " << result << std::endl;
        return false;
    }
    wprintf(L"Hostfxr initialized successfully.\n");

    result = GetDelegateFn(HostFxrHandle, hdt_load_assembly_and_get_function_pointer, (void**)&LoadAssemblyFn);
    if (result != 0 || !LoadAssemblyFn)
    {
        std::cerr << "Failed to get load assembly function with result: " << result << std::endl;
        return false;
    }
    wprintf(L"Load assembly delegate retrieved.\n");

    std::basic_string<char_t> assembly_path = STR("D:\\.mine\\endstone\\FPUCS\\HostLoader\\bin\\DotnetLoader.dll");
    wprintf(L"Assembly path: %ls\n", assembly_path.c_str());
    if (_waccess(assembly_path.c_str(), 0) != 0) {
        std::cerr << "DotnetLoader.dll does not exist or is inaccessible" << std::endl;
        return false;
    }

    const char_t *dotnet_type = STR("DotnetLoader.Entry, DotnetLoader");
    const char_t *dotnet_type_method = STR("Hello");


    result = LoadAssemblyFn(
        assembly_path.c_str(),
        dotnet_type,
        dotnet_type_method,
        nullptr,
        nullptr,
        (void**)&HelloFromDotnet
    );

    if (result != 0)
    {
        std::cerr << "Failed to load assembly with result: " << result << std::endl;
        return false;
    }

    if (!HelloFromDotnet)
    {
        std::cerr << "Failed to get HelloFromDotnet function" << std::endl;
        return false;
    }

    wprintf(L"Successfully loaded HelloFromDotnet function.\n");
    return true;
}

void RunManaged()
{
    HelloFromDotnet();
    CloseHostFn(HostFxrHandle);
}

int main()
{
    if (!InitializeHostFxr())
    {
        std::cerr << "Failed to initialize hostfxr" << std::endl;
        return 1;
    }
    if (!GetDelegates())
    {
        std::cerr << "Failed to get delegates" << std::endl;
        return 1;
    }

    std::basic_string<char_t> configPath;
    if (!TryGetExecutablePath(configPath))
    {
        std::cerr << "Не удалось получить путь к исполняемому файлу." << std::endl;
        return 1;
    }
    configPath += STR("runtime.config.json");

    wprintf(L"configPath: %ls\n", configPath.c_str());

    if (!LoadManagedMethod(configPath.c_str()))
    {
        std::cerr << "Failed to load managed method" << std::endl;
        return 1;
    }

    RunManaged();
    return 0;
}
#include <iostream>
#include <string>
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
    int result = InitForConfigFn(configPath, nullptr, &HostFxrHandle);
    if (result != 0 || !HostFxrHandle) return false;
    result = GetDelegateFn(HostFxrHandle, hdt_load_assembly_and_get_function_pointer, (void**)&LoadAssemblyFn);
    if (result != 0 || !LoadAssemblyFn) return false;
    result = LoadAssemblyFn(
        STR("DotnetLoader.dll"),
        STR("DotnetLoader.Entry, DotnetLoader"),
        STR("HelloFromDotnet"),
        UNMANAGEDCALLERSONLY_METHOD,
        nullptr,
        (void**)&HelloFromDotnet
    );
    return result == 0 && HelloFromDotnet != nullptr;
}

void RunManaged()
{
    HelloFromDotnet();
    CloseHostFn(HostFxrHandle);
}

int main()
{
    if (!InitializeHostFxr()) return 1;
    if (!GetDelegates()) return 1;
    if (!LoadManagedMethod(STR("DotnetLoader.runtimeconfig.json"))) return 1;
    RunManaged();
    return 0;
}
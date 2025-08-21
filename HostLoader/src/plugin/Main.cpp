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

#define string_compare wcscmp

#else
#include <dlfcn.h>
#include <limits.h>

#define STR(s) s
#define CH(c) c
#define DIR_SEPARATOR '/'
#define MAX_PATH PATH_MAX

#define string_compare strcmp

#endif

using CloseHost = hostfxr_close_fn;
using HostHandler = hostfxr_handle;
using GetRuntimeDelegate = hostfxr_get_runtime_delegate_fn;
using InitializeForRuntimeConfig = hostfxr_initialize_for_runtime_config_fn;
using LoadAssemblyAndGetFunctionPtr = load_assembly_and_get_function_pointer_fn;

bool TryGetHostPath(char_t* buffer, size_t buffer_size)
{
    return get_hostfxr_path(buffer, &buffer_size, nullptr) == 0;
}

HMODULE TryLoadLib(const char_t* path)
{
    return LoadLibraryExW(path, nullptr, LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR);
}

int main()
{
    int resultCode = 0;
    HMODULE lib = nullptr;
    char_t hostPath[MAX_PATH];
    const char_t* configPath = STR("DotnetLoader.runtimeconfig.json"); // Changed to match your DLL name

    CloseHost closeHost = nullptr;
    HostHandler hostHandler = nullptr;
    GetRuntimeDelegate getDelegate = nullptr;
    InitializeForRuntimeConfig initForConfig = nullptr;
    LoadAssemblyAndGetFunctionPtr loadAssembly = nullptr;

    if (!TryGetHostPath(hostPath, MAX_PATH))
    {
        std::cerr << "Failed to get hostfxr path" << std::endl;
        return 1;
    }

    lib = TryLoadLib(hostPath);
    if (!lib)
    {
        std::cerr << "Failed to load hostfxr" << std::endl;
        return 1;
    }
    
    closeHost = (CloseHost)GetProcAddress(lib, "hostfxr_close");
    getDelegate = (GetRuntimeDelegate)GetProcAddress(lib, "hostfxr_get_runtime_delegate");
    initForConfig = (InitializeForRuntimeConfig)GetProcAddress(lib, "hostfxr_initialize_for_runtime_config");

    if (!getDelegate || !initForConfig)
    {
        std::cerr << "Failed to get hostfxr delegates" << std::endl;
        return 1;
    }

    resultCode = initForConfig(configPath, nullptr, &hostHandler);

    if (resultCode != 0)
    {
        std::cerr << "Failed to initialize hostfxr context: " << std::hex << resultCode << std::endl;
        return 1;
    }
    
    resultCode = getDelegate(hostHandler, hdt_load_assembly_and_get_function_pointer, (void**)&loadAssembly);
    
    if (resultCode != 0)
    {
        std::cerr << "Failed to get load_assembly_and_get_function_pointer delegate." << std::endl;
        closeHost(hostHandler);
        return 1;
    }

    using HelloFunction = void(void);

    HelloFunction* helloFromDotNet = nullptr;

    resultCode = loadAssembly(
        STR("DotnetLoader.dll"),
        STR("DotnetLoader.Entry, DotnetLoader"),
        STR("HelloFromDotnet"), 
        UNMANAGEDCALLERSONLY_METHOD,
        nullptr,
        (void**)&helloFromDotNet
    );

    if (resultCode != 0 || helloFromDotNet == nullptr)
    {
        std::cerr << "Failed to load assembly or get function pointer. Result code: " << std::hex << resultCode << std::endl;
        closeHost(hostHandler);
        return 1;
    }

    helloFromDotNet();

    closeHost(hostHandler);

    return 0;
}
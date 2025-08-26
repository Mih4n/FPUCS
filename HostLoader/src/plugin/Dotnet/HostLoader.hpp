#pragma once

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

struct PluginEntry
{
    const void* pluginPointer { nullptr };
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

bool TryGetExecutablePath(std::basic_string<char_t>& path);
bool TryGetHostPath(std::basic_string<char_t>& hostPath);
bool TryInitializeHostFxr(const std::basic_string<char_t>& hostPath, HostContext& ctx);
bool TryLoadManagedMethod(const std::basic_string<char_t>& path, HostContext& ctx, EntryPointFunction* entry);
bool TryLoadHostEntry(EntryPointFunction* entry, HostContext& ctx);


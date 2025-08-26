#pragma once

#if defined(_WIN32) || defined(_WIN64)
    #define EXPORT extern "C" __declspec(dllexport)
#else
    #define EXPORT extern "C" __attribute__((visibility("default")))
#endif
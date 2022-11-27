#ifndef TEXAS_HOLDEM_UTIL_SAORI_WINDOWS_H_
#define TEXAS_HOLDEM_UTIL_SAORI_WINDOWS_H_

#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

    __declspec(dllexport) BOOL        __cdecl load(HGLOBAL h, long len);
    __declspec(dllexport) BOOL        __cdecl unload();
    __declspec(dllexport) HGLOBAL __cdecl request(HGLOBAL h, long *len);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // TEXAS_HOLDEM_UTIL_SAORI_WINDOWS_H_

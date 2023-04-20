// MeldHelper.cpp : 
//
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>
#include <cstdlib>
#include <ctime>

#define HELPER_TITLE L"MeldHelper"
#define TMP_FILE_NAME L"_meld_helper"
#define EXCUTOR_NAME L"Meld.exe"
#define TIME_OUT 180 // timeout seconds for picking 2 targets

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    [[unlikely]] if (!lpCmdLine[0])
        return 0;
    WCHAR tmpPath[256];
    GetTempPath(256, tmpPath);
    lstrcatW(tmpPath, TMP_FILE_NAME);
    auto hHelperFile = CreateFile(tmpPath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    [[unlikely]] if (INVALID_HANDLE_VALUE == hHelperFile)
    {
        MessageBox(NULL, tmpPath, L"[" HELPER_TITLE L"]" L" Fail to create file", MB_OK);
        return 0;
    }
    auto tNow = time(nullptr);

    char buf[1024];
    DWORD rByte;
    auto ret = ReadFile(hHelperFile, buf, 512, &rByte, nullptr);
    SetFilePointer(hHelperFile, 0, NULL, FILE_BEGIN); // seek to beginning
    [[likely]] if (ret && rByte)
    {
        time_t tLast = _atoi64(buf);
        auto timeDelta = tNow - tLast;
        if (timeDelta < TIME_OUT)
        {
            WCHAR exePath[256];
            GetModuleFileName(NULL, exePath, 255);
            // get module directory
            for (LPWSTR pEnd = exePath, p = exePath; ; p++)
            {
                [[unlikely]] if (*p == '\\') {
                    pEnd = p; continue;
                }
                [[unlikely]] if (!(*p)) {
                    *pEnd = 0; break;
                }
            }

            SetEndOfFile(hHelperFile);
            DWORD wByte;
            WriteFile(hHelperFile, "0", 1, &wByte, NULL);
            LPWSTR lpParam = LPWSTR(buf + strlen(buf) + 1); // point to privious written lpCmdLine (target 1)
            *LPWSTR(buf + rByte) = L' '; // write a space
            rByte += sizeof(WCHAR);
            lstrcpyW(LPWSTR(buf + rByte), lpCmdLine); // write this lpCmdLine (target 2)
            lstrcatW(exePath, L"\\" EXCUTOR_NAME);
            ShellExecute(NULL, L"open", exePath, lpParam, nullptr, SW_SHOW);
            [[unlikely]] if (auto err = GetLastError())
            {
                MessageBox(NULL, exePath, L"[" HELPER_TITLE L"]" L" Excutor Error", MB_OK);
            }

            goto _Exit0;
        }
    }
    {
        // write new time and lpCmdLine (target 1)
        _i64toa_s(tNow, buf, 512, 10);
        auto len = strlen(buf) + 1;
        auto cmdLen = 2 * wcslen(lpCmdLine);
        memcpy(buf + len, lpCmdLine, cmdLen);
        SetEndOfFile(hHelperFile);
        WriteFile(hHelperFile, buf, (DWORD)(len + cmdLen), &rByte, NULL);
    }
_Exit0:
    CloseHandle(hHelperFile);
    return 0;
}
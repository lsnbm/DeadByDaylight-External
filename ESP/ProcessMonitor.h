#pragma once

#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <vector>
#include <string>
#include <unordered_map>
#include "imgui.h"

#pragma comment(lib, "psapi.lib")

class ProcessMonitor {
public:
    ProcessMonitor() : lastUpdateTime(0), updateInterval(1000) {} // 默认1秒更新间隔

    struct ProcessInfo {
        DWORD pid;
        std::wstring name;
        SIZE_T memoryUsage;      // bytes
        double cpuUsage;         // percent
        DWORD threadCount;
        std::wstring windowTitle;
    };

    // 设置更新间隔(毫秒)
    void SetUpdateInterval(DWORD intervalMs) {
        updateInterval = intervalMs;
    }

    // 获取指定进程信息 (带更新间隔控制)
    bool GetProcessInfo(const std::wstring& processName, ProcessInfo& info) {
        DWORD currentTime = GetTickCount();

        // 检查是否需要更新
        if (currentTime - lastUpdateTime < updateInterval && cachedInfo.pid != 0) {
            info = cachedInfo;
            return true;
        }

        HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hSnap == INVALID_HANDLE_VALUE) return false;

        PROCESSENTRY32W pe = { sizeof(pe) };
        if (Process32FirstW(hSnap, &pe)) {
            do {
                if (_wcsicmp(pe.szExeFile, processName.c_str()) == 0) {
                    info.pid = pe.th32ProcessID;
                    info.name = pe.szExeFile;
                    info.threadCount = pe.cntThreads;

                    GetMemoryUsage(info);
                    GetCPUUsage(info);
                    GetWindowTitle(info);

                    // 缓存结果和更新时间
                    cachedInfo = info;
                    lastUpdateTime = currentTime;

                    CloseHandle(hSnap);
                    return true;
                }
            } while (Process32NextW(hSnap, &pe));
        }
        CloseHandle(hSnap);
        return false;
    }

private:
    DWORD lastUpdateTime;
    DWORD updateInterval;  // 更新间隔(毫秒)
    ProcessInfo cachedInfo; // 缓存的进程信息

    static std::unordered_map<DWORD, ULONGLONG> lastProcessTime;
    static std::unordered_map<DWORD, ULONGLONG> lastSystemTime;

    void GetMemoryUsage(ProcessInfo& info) {
        info.memoryUsage = 0;
        HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, info.pid);
        if (hProc) {
            PROCESS_MEMORY_COUNTERS pmc;
            if (GetProcessMemoryInfo(hProc, &pmc, sizeof(pmc))) {
                info.memoryUsage = pmc.WorkingSetSize;
            }
            CloseHandle(hProc);
        }
    }

    void GetCPUUsage(ProcessInfo& info) {
        info.cpuUsage = 0.0;

        HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, info.pid);
        if (!hProc) return;

        FILETIME createTime, exitTime, kernelTime, userTime;
        if (GetProcessTimes(hProc, &createTime, &exitTime, &kernelTime, &userTime)) {
            ULARGE_INTEGER kernel, user, system;
            kernel.LowPart = kernelTime.dwLowDateTime;
            kernel.HighPart = kernelTime.dwHighDateTime;
            user.LowPart = userTime.dwLowDateTime;
            user.HighPart = userTime.dwHighDateTime;

            GetSystemTimeAsFileTime(&createTime); // 重用变量获取系统时间
            system.LowPart = createTime.dwLowDateTime;
            system.HighPart = createTime.dwHighDateTime;

            ULONGLONG currentProcessTime = kernel.QuadPart + user.QuadPart;
            ULONGLONG currentSystemTime = system.QuadPart;

            auto lastProcIt = lastProcessTime.find(info.pid);
            auto lastSysIt = lastSystemTime.find(info.pid);

            if (lastProcIt != lastProcessTime.end() && lastSysIt != lastSystemTime.end()) {
                ULONGLONG procTimeDiff = currentProcessTime - lastProcIt->second;
                ULONGLONG sysTimeDiff = currentSystemTime - lastSysIt->second;

                if (sysTimeDiff > 0) {
                    info.cpuUsage = (double)procTimeDiff / (double)sysTimeDiff * 100.0;
                }
            }

            lastProcessTime[info.pid] = currentProcessTime;
            lastSystemTime[info.pid] = currentSystemTime;
        }
        CloseHandle(hProc);
    }

    void GetWindowTitle(ProcessInfo& info) {
        struct EnumData {
            DWORD pid;
            std::wstring title;
        } data{ info.pid, L"" };

        EnumWindows([](HWND hwnd, LPARAM lp) -> BOOL {
            auto d = reinterpret_cast<EnumData*>(lp);
            DWORD pid;
            GetWindowThreadProcessId(hwnd, &pid);

            if (pid == d->pid && IsWindowVisible(hwnd)) {
                wchar_t buf[256];
                if (GetWindowTextW(hwnd, buf, _countof(buf)) > 0) {
                    d->title = buf;
                    return FALSE;
                }
            }
            return TRUE;
            }, reinterpret_cast<LPARAM>(&data));

        info.windowTitle = data.title;
    }
};

// 静态成员定义
std::unordered_map<DWORD, ULONGLONG> ProcessMonitor::lastProcessTime;
std::unordered_map<DWORD, ULONGLONG> ProcessMonitor::lastSystemTime;

// Helper函数
static std::string WStringToUtf8(const std::wstring& wstr) {
    if (wstr.empty()) return std::string();
    int size = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
    if (size <= 0) return std::string();
    std::string str(size - 1, '\0');
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], size, NULL, NULL);
    return str;
}

static std::string FormatBytes(SIZE_T bytes) {
    if (bytes >= 1024 * 1024 * 1024) {
        return std::to_string(bytes / (1024 * 1024 * 1024)) + " GB";
    }
    else if (bytes >= 1024 * 1024) {
        return std::to_string(bytes / (1024 * 1024)) + " MB";
    }
    else if (bytes >= 1024) {
        return std::to_string(bytes / 1024) + " KB";
    }
    return std::to_string(bytes) + " B";
}

// 绘制函数
static void DrawProcessInfo(const ProcessMonitor::ProcessInfo& info, ImDrawList* drawList, ImVec2 pos,float size, ImU32 color) {

    std::string text = "Process: " + WStringToUtf8(info.name) +
        " | PID: " + std::to_string(info.pid) +
        " | Memory: " + FormatBytes(info.memoryUsage) +
        " | CPU: " + std::to_string((int)info.cpuUsage*0.1) + "%" +
        " | Threads: " + std::to_string(info.threadCount);

    if (!info.windowTitle.empty()) {
        text += " | Window: " + WStringToUtf8(info.windowTitle);
    }

    drawList->AddText(NULL,size,pos, color, text.c_str());
}

// 使用示例:
/*
ProcessMonitor monitor;
monitor.SetUpdateInterval(2000); // 设置2秒更新一次

ProcessMonitor::ProcessInfo info;
if (monitor.GetProcessInfo(L"notepad.exe", info)) {
    DrawProcessInfo(info, ImVec2(10, 10));
}
*/
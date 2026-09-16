#include "process_manager.hpp"
#include <tlhelp32.h>
#include <algorithm>

std::vector<ProcessEntry> ProcessManager::GetRunningProcesses() {
    std::vector<ProcessEntry> processes;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE) return processes;

    PROCESSENTRY32W pe;
    pe.dwSize = sizeof(pe);

    if (Process32FirstW(hSnap, &pe)) {
        do {
            processes.push_back({ pe.th32ProcessID, pe.szExeFile });
        } while (Process32NextW(hSnap, &pe));
    }

    CloseHandle(hSnap);

    std::sort(processes.begin(), processes.end(), [](const ProcessEntry& a, const ProcessEntry& b) {
        return a.name < b.name;
    });

    return processes;
}

bool ProcessManager::SetPriorityByPID(DWORD pid, DWORD priorityClass) {
    HANDLE hProc = OpenProcess(PROCESS_SET_INFORMATION | PROCESS_QUERY_INFORMATION, FALSE, pid);
    if (!hProc) return false;

    bool success = SetPriorityClass(hProc, priorityClass);
    CloseHandle(hProc);
    return success;
}

bool ProcessManager::SetCpuAffinity(DWORD pid, DWORD_PTR affinityMask) {
    HANDLE hProc = OpenProcess(PROCESS_SET_INFORMATION | PROCESS_QUERY_INFORMATION, FALSE, pid);
    if (!hProc) return false;

    bool success = SetProcessAffinityMask(hProc, affinityMask);
    CloseHandle(hProc);
    return success;
}

bool ProcessManager::ForceHighPerformancePower() {
    HANDLE hProcess = GetCurrentProcess();
    return SetPriorityClass(hProcess, HIGH_PRIORITY_CLASS) != 0;
}

void ProcessManager::PurgeSystemMemory() {
    SetProcessWorkingSetSize(GetCurrentProcess(), -1, -1);
}

bool ProcessManager::OptimizeProcess(DWORD pid) {
    if (pid == 0) return false;

    if (!SetPriorityByPID(pid, HIGH_PRIORITY_CLASS)) {
        return false;
    }

    SYSTEM_INFO si;
    GetSystemInfo(&si);

    DWORD_PTR mask = 0;
    for (DWORD i = 0; i < si.dwNumberOfProcessors; ++i) {
        mask |= (DWORD_PTR(1) << i);
    }

    return SetCpuAffinity(pid, mask);
}
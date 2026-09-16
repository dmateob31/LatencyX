#ifndef PROCESS_MANAGER_HPP
#define PROCESS_MANAGER_HPP

#include <windows.h>
#include <string>
#include <vector>

struct ProcessEntry {
    DWORD pid;
    std::wstring name;
};

class ProcessManager {
public:
    static std::vector<ProcessEntry> GetRunningProcesses();
    static bool SetPriorityByPID(DWORD pid, DWORD priorityClass);
    static bool SetCpuAffinity(DWORD pid, DWORD_PTR affinityMask);

    // Funciones que usa main.cpp
    static bool ForceHighPerformancePower();
    static void PurgeSystemMemory();
    static bool OptimizeProcess(DWORD pid);
};

#endif
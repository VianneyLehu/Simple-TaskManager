#pragma once
#include <windows.h>
#include <psapi.h>
#include <string>
#include <vector>

class ProcessInfo {

private:
    DWORD pid;
    std::string name;
    DWORD memoryusageMB;
    HANDLE HandleProcess;

public:
    //getter
    DWORD GetPID() const;
    const std::string& GetName() const;
    DWORD GetMemoryUsageMB() const;
    HANDLE GetHandle() const;
    void GetProcessInfo();

    //setter
    void SetPID(DWORD p);
    void SetSetName(std::string n);
    void SetMemoryUsageMB(DWORD m);
    void SetHandle(HANDLE h);


};


void GenerateProcessInfo(std::vector<ProcessInfo>& P);

std::string GetNameByHandle(HANDLE hprocess);





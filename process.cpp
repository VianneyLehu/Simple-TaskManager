    #include "process.h"
    #include <vector>
    #include <iostream>
    #include <psapi.h>
    #include <errhandlingapi.h>
    #include <string>



    //Getter
    DWORD ProcessInfo::GetPID() const {
        return pid;
    }

    const std::string& ProcessInfo::GetName() const{
        return name;
    }

    DWORD ProcessInfo::GetMemoryUsageMB() const {
        return memoryusageMB;
    }

    HANDLE ProcessInfo::GetHandle() const {
        return HandleProcess;
    }



    //Setter
    void ProcessInfo::SetPID(DWORD p){
        pid = p;
    }

    void ProcessInfo::SetSetName(std::string n){
        name = n;
    }


    void ProcessInfo::SetMemoryUsageMB(DWORD m){
        memoryusageMB = m;
    }

    void ProcessInfo::SetHandle(HANDLE h){
        HandleProcess = h;
    }

    void ProcessInfo::GetProcessInfo(){
        std::cout<<"PID: "<<pid<<"\n";
        std::cout<<"name: "<<name<<"\n";
        std::cout<<"Memory usage: "<<memoryusageMB<<" MB"<<"\n";
        std::cout<<"Handle: "<<HandleProcess<<"\n";

    }

    std::string GetNameByHandle(HANDLE hprocess){
        HMODULE hmodule;
        DWORD sizeModule;
        wchar_t name[1024];
        if(EnumProcessModules(hprocess, &hmodule, sizeof(hmodule), &sizeModule)!= NULL){
            DWORD nbmodules = sizeModule / sizeof(DWORD);
            GetModuleBaseName(hprocess, hmodule, name, sizeof(name));
        }
        std::wstring ws(name);
        std::string str(ws.begin(), ws.end());

        return str;
    }

    DWORD GetMemoryUsageByHandle(HANDLE hprocess){
        PROCESS_MEMORY_COUNTERS pmc;
        if(GetProcessMemoryInfo(hprocess, &pmc, sizeof(pmc)) != 0){
            return (pmc.WorkingSetSize/ (1024 * 1024));
        }

        return -1;
    }



    void GenerateProcessInfo(std::vector<ProcessInfo> &P){

        DWORD processList[1024], nbProcesses, sizeProcesslist;

        if(!EnumProcesses(processList, sizeof(processList), &sizeProcesslist)){
            std::cout<<"Error with enumprocesses"<<std::endl; //change thisZ
        }else{
            nbProcesses = sizeProcesslist / sizeof(DWORD);
            for(DWORD i = 0; i < nbProcesses; i++){
                DWORD pid = processList[i];
                ProcessInfo PI;

                PI.SetPID(pid);
                HANDLE hprocess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
                PI.SetHandle(hprocess);
                if(hprocess != NULL){
                    if(GetNameByHandle(hprocess) != ""){
                        PI.SetSetName(GetNameByHandle(hprocess));
                        PI.SetMemoryUsageMB(GetMemoryUsageByHandle(hprocess));
                        P.push_back(PI);
                    }

                }
                CloseHandle(hprocess);

            }

        }


    }





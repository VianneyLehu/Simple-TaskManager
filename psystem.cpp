#include "psystem.h"



/*
template <typename T>
LPCWSTR convertToSize(T size) {
    LPCWSTR lpSize = (LPCWSTR)VirtualAlloc(nullptr, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    if (lpSize != 0) {
        if (StrFormatByteSizeW((LONGLONG)size, (PWSTR)lpSize, MAX_PATH) == NULL) {
            VirtualFree((LPVOID)lpSize, 0x0, MEM_RELEASE);
            lpSize = nullptr;
        }
    }


    return lpSize;
}


void displayPsystem(PSYSTEM_PROCESS_INFORMATION p ) {


    std::cout << "-----------------------------------------------" << std::endl;
    std::wcout << L"[id] : [" << HandleToULong(p->UniqueProcessId) << "]" << std::endl;
    std::wcout << L"[Name] : [" << (p->ImageName.Buffer ? p->ImageName.Buffer : L"") << "]" << std::endl;
    std::wcout << L"[Virtual size] : [" << convertToSize(p->VirtualMemoryCounters.VirtualSize) << "]" << std::endl;
    std::wcout << L"[Working Set Size] : [" << convertToSize(p->VirtualMemoryCounters.WorkingSetSize) << " ]" << std::endl;

}


int main(VOID) {

    std::locale::global(std::locale(""));

    DWORD dwRet;
    DWORD dwSize = 0x0;
    NTSTATUS dwstatus;
    PSYSTEM_PROCESS_INFORMATION p = nullptr;


    while (TRUE) {
        if (p != nullptr) VirtualFree(p, 0x0, MEM_RELEASE);
        p = (PSYSTEM_PROCESS_INFORMATION)VirtualAlloc(nullptr, dwSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        dwstatus = NtQuerySystemInformation(SystemProcessInformation, (PVOID)p, (ULONG)dwSize, &dwRet);

        if (dwstatus == STATUS_SUCCESS) break;

        else if (dwstatus != STATUS_INFO_LENGTH_MISMATCH) {

            VirtualFree(p, 0x0, MEM_RELEASE);
            p = nullptr;
            std::cout << "[!] Error fetching process information" << std::endl;
            return 0x1;
        }


        dwSize = dwRet + (2<<12);


    }

    do {

        displayPsystem(p);

        p = (PSYSTEM_PROCESS_INFORMATION)((PBYTE)p + p->NextEntryOffset);

    }

    while (p->NextEntryOffset != 0);

    VirtualFree(p, 0x0, MEM_RELEASE);

    p = nullptr;


    return 0x0;
}



std::wstring convertToSize(SIZE_T size) {
    WCHAR buffer[MAX_PATH];
    if (StrFormatByteSizeW(size, buffer, MAX_PATH) == NULL) {
        return L"";
    }
    return std::wstring(buffer);
}

// Function to display process information in a table
void displayPsystem(QTableWidget* tableWidget, PSYSTEM_PROCESS_INFORMATION p) {
    int row = tableWidget->rowCount();
    tableWidget->insertRow(row);

    QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(HandleToULong(p->UniqueProcessId)));
    QTableWidgetItem* nameItem = new QTableWidgetItem(QString::fromWCharArray(p->ImageName.Buffer ? p->ImageName.Buffer : L""));
    QTableWidgetItem* virtualSizeItem = new QTableWidgetItem(QString::fromStdWString(convertToSize(p->VirtualMemoryCounters.VirtualSize)));
    QTableWidgetItem* workingSetSizeItem = new QTableWidgetItem(QString::fromStdWString(convertToSize(p->VirtualMemoryCounters.WorkingSetSize)));

    tableWidget->setItem(row, 0, idItem);
    tableWidget->setItem(row, 1, nameItem);
    tableWidget->setItem(row, 2, virtualSizeItem);
    tableWidget->setItem(row, 3, workingSetSizeItem);

}

int generatePsystemInfo(){
    DWORD dwRet;
    DWORD dwSize = 0x0;
    NTSTATUS dwstatus;
    PSYSTEM_PROCESS_INFORMATION p = nullptr;

    while (true) {
        if (p != nullptr) VirtualFree(p, 0x0, MEM_RELEASE);
        p = (PSYSTEM_PROCESS_INFORMATION)VirtualAlloc(nullptr, dwSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        dwstatus = NtQuerySystemInformation(SystemProcessInformation, (PVOID)p, (ULONG)dwSize, &dwRet);

        if (dwstatus == STATUS_SUCCESS) break;
        else if (dwstatus != STATUS_INFO_LENGTH_MISMATCH) {
            VirtualFree(p, 0x0, MEM_RELEASE);
            p = nullptr;
            std::cerr << "[!] Error fetching process information" << std::endl;
            return 0x1;
        }
        dwSize = dwRet + (2 << 12);
    }

    do {
        displayPsystem(tableWidget, p);
        p = (PSYSTEM_PROCESS_INFORMATION)((PBYTE)p + p->NextEntryOffset);
    } while (p->NextEntryOffset != 0);

    VirtualFree(p, 0x0, MEM_RELEASE);

    return 0x0;
}

*/

#ifndef PCH_H
#define PCH_H

#endif // PCH_H

#pragma once
#include <windows.h>
#include <ShlWapi.h>
#include <winternl.h>
#include <iostream>
#include <string>
#include <locale>


constexpr unsigned int STATUS_SUCCESS = 0x0;
constexpr unsigned int STATUS_INFO_LENGTH_MISMATCH = 0x0C0000004;


EXTERN_C NTSTATUS NTAPI NtQuerySystemInformation(
    IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
    OUT PVOID               SystemInformation,
    IN ULONG                SystemInformationLength,
    OUT PULONG              ReturnLength OPTIONAL
    );



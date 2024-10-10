#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <vector>
#include <stdlib.h>
#include <math.h>
#include "framework.h"

DWORD GetModuleBaseAddress(TCHAR* lpszModuleName, DWORD pID) {
    DWORD dwModuleBaseAddress = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID);
    MODULEENTRY32 ModuleEntry32 = { 0 };
    ModuleEntry32.dwSize = sizeof(MODULEENTRY32);

    if (Module32First(hSnapshot, &ModuleEntry32))
    {
        do {
            if (_tcscmp(ModuleEntry32.szModule, lpszModuleName) == 0)
            {
                dwModuleBaseAddress = (DWORD)ModuleEntry32.modBaseAddr;
                break;
            }
        } while (Module32Next(hSnapshot, &ModuleEntry32));


    }
    CloseHandle(hSnapshot);
    return dwModuleBaseAddress;
}
DWORD GetPointerAddress(HWND hwnd, DWORD gameBaseAddr, DWORD address, std::vector<DWORD> offsets) {
    DWORD pID = NULL;
    GetWindowThreadProcessId(hwnd, &pID);
    HANDLE phandle = NULL;
    phandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);
    if (phandle == INVALID_HANDLE_VALUE || phandle == NULL);

    DWORD offset_null = NULL;
    ReadProcessMemory(phandle, (LPVOID*)(gameBaseAddr + address), &offset_null, sizeof(offset_null), 0);
    DWORD pointeraddress = offset_null;
    for (int i = 0; i < offsets.size() - 1; i++)
    {
        ReadProcessMemory(phandle, (LPVOID*)(pointeraddress + offsets.at(i)), &pointeraddress, sizeof(pointeraddress), 0);
    }
    return pointeraddress += offsets.at(offsets.size() - 1);
}

DWORD WINAPI MainCore(HMODULE hModule) {
    bool RunStage = 0;
    int ActiveCharacter = 0;
    float Acceleration = 0.0f;
    float TimeTick = 0.0f;
    const float CoefficientOfX = 20 / (2.0f * 2.0f); // MaxAcceleration / TimeToMaxAcceleration^2

    HWND hwnd_SonicHeroesTM = FindWindowA(NULL, "SONIC HEROES(TM)");

    while (hwnd_SonicHeroesTM == NULL) {
        hwnd_SonicHeroesTM = FindWindowA(NULL, "SONIC HEROES(TM)");
    }

    DWORD ProcessIDSonicHeroes = NULL;
    GetWindowThreadProcessId(hwnd_SonicHeroesTM, &ProcessIDSonicHeroes);
    HANDLE HandleSonicHeroes = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessIDSonicHeroes);
    char SonicHeroesGameModule[] = "Tsonic_win.exe";
    DWORD SonicHeroesMainAdress = GetModuleBaseAddress(_T(SonicHeroesGameModule), ProcessIDSonicHeroes);

    DWORD AccelerationMainAdress = 0x005CE820;
    DWORD ActiveCharacterMainAdress = 0x006778AC;

    std::vector<DWORD> AccelerationOffset{ 0xDC };
    std::vector<DWORD> ActiveCharacterOffset{ 0x18, 0x148 };

    while (true) {
        ReadProcessMemory(HandleSonicHeroes, (PBYTE*)0x007C6BD4, &RunStage, sizeof(bool), 0);

        if (RunStage == 0) { // Stage Run.

            DWORD ActiveCharacterAdress = GetPointerAddress(hwnd_SonicHeroesTM, SonicHeroesMainAdress, ActiveCharacterMainAdress, ActiveCharacterOffset);
            ReadProcessMemory(HandleSonicHeroes, (PBYTE*)ActiveCharacterAdress, &ActiveCharacter, sizeof(int), 0);

            if (ActiveCharacter == 0) { // Select Speed Character.

                DWORD AccelerationAdress = GetPointerAddress(hwnd_SonicHeroesTM, SonicHeroesMainAdress, AccelerationMainAdress, AccelerationOffset);
                ReadProcessMemory(HandleSonicHeroes, (PBYTE*)AccelerationAdress, &Acceleration, sizeof(float), 0);

                TimeTick = sqrt(Acceleration / CoefficientOfX);

                if (TimeTick >= 2.0f) { // TimeToMaxAcceleration
                    TimeTick = 1.99f; //TimeToMaxAcceleration - 0.01f
                }
                else {
                }

                TimeTick += 0.01f; // The program is approximately 100 frames per second so the time will increase by 0,01 seconds each time.

                Acceleration = CoefficientOfX * TimeTick * TimeTick; // y = ax^2 -- MaxAcceleration = CoefficientOfX * Time^2

                if (GetAsyncKeyState(0x43)) {
                    WriteProcessMemory(HandleSonicHeroes, (PBYTE*)AccelerationAdress, &Acceleration, sizeof(float), 0);
                }
                else {
                }

            }
            else{
            }

        }

        else { // Stage Not Run.

        }

        Sleep(10); // Waiting time of the code. Approximately 100 frames per second.

    }

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainCore, hModule, 0, nullptr));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}


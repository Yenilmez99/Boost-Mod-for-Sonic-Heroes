#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <vector>
#include <stdlib.h>
#include <math.h>
#include <Xinput.h>
#include <string>
#include <fstream>
#include <sstream>
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
    bool RunStage = 0, GamepadKeyPress = 0, isTrigger = 0;
    int ButtonKey;
    BYTE TriggerKey;
    float Acceleration = 0.0f;
    float TimeTick = 0.0f;

    float MaxAcceleration = 20.0f;
    float TimeToMaxAcceleration = 2.0f;

    std::ifstream File("Y99 Mods Config File.txt");
    std::string Line;
    short ReadKey = 12;
    if (!File.is_open())
        ReadKey = 12;
    else {
        for (int i = 0; i < 7; i++) {
            std::getline(File, Line);
            if (i == 1) {
                std::stringstream ss(Line);
                std::string Key;
                ss >> Key >> ReadKey;
            }
            if (i == 2) {
                std::stringstream ss(Line);
                std::string Key;
                ss >> Key >> MaxAcceleration;
            }
            if (i == 3) {
                std::stringstream ss(Line);
                std::string Key;
                ss >> Key >> TimeToMaxAcceleration;
            }
        }
        if (ReadKey <= 12 && ReadKey >= 1);
        else
            ReadKey = 12;

        if (MaxAcceleration <= 100.0f && MaxAcceleration >= 0.5f);
        else
            MaxAcceleration = 20.0f;

        if (TimeToMaxAcceleration <= 120.0f && TimeToMaxAcceleration >= 1.0f);
        else
            TimeToMaxAcceleration = 2.0f;
    }
    File.close();

    const float CoefficientOfX = MaxAcceleration / (TimeToMaxAcceleration * TimeToMaxAcceleration); // MaxAcceleration / TimeToMaxAcceleration^2

    XINPUT_STATE state;
    DWORD dwResult;

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
    std::vector<DWORD> AccelerationOffset{ 0xDC };
    DWORD AccelerationAdress;

    while (hwnd_SonicHeroesTM != NULL) {
        ReadProcessMemory(HandleSonicHeroes, (PBYTE*)0x007C6BD4, &RunStage, sizeof(bool), 0);

        dwResult = XInputGetState(0, &state);
        if (dwResult == ERROR_SUCCESS) {
            switch (ReadKey) {
            case 1:
                isTrigger = 0;
                ButtonKey = XINPUT_GAMEPAD_A;
                break;
            case 2:
                isTrigger = 0;
                ButtonKey = XINPUT_GAMEPAD_B;
                break;
            case 3:
                isTrigger = 0;
                ButtonKey = XINPUT_GAMEPAD_X;
                break;
            case 4:
                isTrigger = 0;
                ButtonKey = XINPUT_GAMEPAD_Y;
                break;
            case 5:
                isTrigger = 0;
                ButtonKey = XINPUT_GAMEPAD_LEFT_SHOULDER;
                break;
            case 6:
                isTrigger = 0;
                ButtonKey = XINPUT_GAMEPAD_RIGHT_SHOULDER;
                break;
            case 7:
                isTrigger = 0;
                ButtonKey = XINPUT_GAMEPAD_BACK;
                break;
            case 8:
                isTrigger = 0;
                ButtonKey = XINPUT_GAMEPAD_START;
                break;
            case 9:
                isTrigger = 0;
                ButtonKey = XINPUT_GAMEPAD_LEFT_THUMB;
                break;
            case 10:
                isTrigger = 0;
                ButtonKey = XINPUT_GAMEPAD_RIGHT_THUMB;
                break;
            case 11:
                isTrigger = 1;
                TriggerKey = state.Gamepad.bLeftTrigger;
                break;
            case 12:
                isTrigger = 1;
                TriggerKey = state.Gamepad.bRightTrigger;
                break;
            default:
                isTrigger = 1;
                TriggerKey = state.Gamepad.bRightTrigger;
                break;
            }
            if (isTrigger != 1) {
                if ((state.Gamepad.wButtons & ButtonKey) != 0)
                    GamepadKeyPress = 1;
                else
                    GamepadKeyPress = 0;
            }
            else {
                if (TriggerKey != 0)
                    GamepadKeyPress = 1;
                else
                    GamepadKeyPress = 0;
            }

        }
        else
            GamepadKeyPress = 0;
        
        if (RunStage == 0) { // Stage Run.
            AccelerationAdress = GetPointerAddress(hwnd_SonicHeroesTM, SonicHeroesMainAdress, AccelerationMainAdress, AccelerationOffset);
            ReadProcessMemory(HandleSonicHeroes, (PBYTE*)AccelerationAdress, &Acceleration, sizeof(float), 0);

            if (Acceleration > 0.01f) {
                TimeTick = sqrt(Acceleration / CoefficientOfX);

                if (TimeTick >= TimeToMaxAcceleration) // TimeToMaxAcceleration
                    TimeTick = TimeToMaxAcceleration - 0.016f; //TimeToMaxAcceleration - 0.016f

                TimeTick += 0.016f; // The program is approximately 60 frames per second so the time will increase by 0,016 seconds each time.

                Acceleration = CoefficientOfX * TimeTick * TimeTick; // y = ax^2 -- MaxAcceleration = CoefficientOfX * Time^2

                if (GetAsyncKeyState(0x43) || GamepadKeyPress)
                    WriteProcessMemory(HandleSonicHeroes, (PBYTE*)AccelerationAdress, &Acceleration, sizeof(float), 0);
            }

        }

        Sleep(16); // Waiting time of the code. Approximately 60 frames per second.
        hwnd_SonicHeroesTM = FindWindowA(NULL, "SONIC HEROES(TM)");

    }
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
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


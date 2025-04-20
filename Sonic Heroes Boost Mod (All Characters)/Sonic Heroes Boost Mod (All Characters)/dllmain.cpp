#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <vector>
#include <stdlib.h>
#include <math.h>
#include <Xinput.h>
#include <string>
#include <fstream>
#include "framework.h"
#include "json.hpp"

#define KeyboardButtonKey 0x43

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

std::string GetDllPath(std::string DLLName) {
    HMODULE hModule = NULL;
    char path[MAX_PATH];
    std::string DLLPath = "";
    LPCSTR DLLNameLPCSTR = DLLName.c_str();

    GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_PIN, DLLNameLPCSTR, &hModule);
    GetModuleFileNameA(hModule, path, MAX_PATH);
    DLLPath = path;

    if (DLLPath.find_last_of("\\/") != std::string::npos)
        DLLPath = DLLPath.substr(0, DLLPath.find_last_of("\\/"));

    return DLLPath;
}


DWORD WINAPI MainCore(HMODULE hModule) {
    bool RunStage = 0, GamepadKeyPress = 0, isTrigger = 0;
    int ButtonKey;
    BYTE TriggerKey;
    UINT8 Aura[3] = { NULL,NULL,NULL };
    float Acceleration = 0.0f;
    float TimeTick = 0.0f;
    float TeamBlastBar = 0.0f;

    float MaxAcceleration = 20.0f;
    float TimeToMaxAcceleration = 2.0f;

    // Find DLL Location
    std::ifstream File(GetDllPath("Sonic Heroes Boost Mod (All Characters).dll") + "\\..\\..\\User\\Mods\\sonicheroes.mechanic.boostforallsh\\Config.json");
    short ReadKey = 12;
    if (!File.is_open())
        ReadKey = 12;

    nlohmann::json configData;
    File >> configData;

    ReadKey = configData["Integer"];
    MaxAcceleration = configData["Float"];
    TimeToMaxAcceleration = configData["Float2"];

    File.close();

    if (ReadKey <= 12 && ReadKey >= 1);
    else
        ReadKey = 12;

    if (MaxAcceleration <= 100.0f && MaxAcceleration >= 0.5f);
    else
        MaxAcceleration = 20.0f;

    if (TimeToMaxAcceleration <= 120.0f && TimeToMaxAcceleration >= 1.0f);
    else
        TimeToMaxAcceleration = 2.0f;

    const float CoefficientOfX = MaxAcceleration / (TimeToMaxAcceleration * TimeToMaxAcceleration); // MaxAcceleration / TimeToMaxAcceleration^2

    XINPUT_STATE state;
    DWORD dwResult;

    HWND hwnd_SonicHeroesTM = FindWindowA(NULL, "SONIC HEROES(TM)");

    while (hwnd_SonicHeroesTM == NULL)
        hwnd_SonicHeroesTM = FindWindowA(NULL, "SONIC HEROES(TM)");

    DWORD ProcessIDSonicHeroes = NULL;
    GetWindowThreadProcessId(hwnd_SonicHeroesTM, &ProcessIDSonicHeroes);
    HANDLE HandleSonicHeroes = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessIDSonicHeroes);
    char SonicHeroesGameModule[] = "Tsonic_win.exe";
    DWORD SonicHeroesMainAdress = GetModuleBaseAddress(_T(SonicHeroesGameModule), ProcessIDSonicHeroes);

    DWORD AccelerationMainAdress = 0x005CE820;
    DWORD AuraMainAdress = 0x0064B1B0;
    std::vector<DWORD> AccelerationOffset{ 0xDC };
    std::vector<DWORD> AuraOffset{ 0x1bd };
    DWORD AccelerationAdress;
    DWORD AuraAddress[3]; // 3 Character

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
    }
    else
        GamepadKeyPress = 0;

    while (hwnd_SonicHeroesTM != NULL) {
        ReadProcessMemory(HandleSonicHeroes, (PBYTE*)0x007C6BD4, &RunStage, sizeof(bool), 0);
        
        if (RunStage == 0) { // Stage Run.

            AccelerationAdress = GetPointerAddress(hwnd_SonicHeroesTM, SonicHeroesMainAdress, AccelerationMainAdress, AccelerationOffset);
            ReadProcessMemory(HandleSonicHeroes, (PBYTE*)AccelerationAdress, &Acceleration, sizeof(float), 0);

            ReadProcessMemory(HandleSonicHeroes, (PBYTE*)0x009DD72C, &TeamBlastBar, sizeof(float), 0);

            for (UINT8 k = 0; k < 3; k++) {
                AuraAddress[k] = GetPointerAddress(hwnd_SonicHeroesTM, SonicHeroesMainAdress, (AuraMainAdress + k * 0x4), AuraOffset);
                ReadProcessMemory(HandleSonicHeroes, (PBYTE*)AuraAddress[k], &Aura[k], sizeof(UINT8), 0);
            }

            if (Acceleration > 0.01f) {

                dwResult = XInputGetState(0, &state);
                if (dwResult == ERROR_SUCCESS) {
                    if (isTrigger != 1) {
                        if ((state.Gamepad.wButtons & ButtonKey) != 0)
                            GamepadKeyPress = 1;
                        else
                            GamepadKeyPress = 0;
                    }
                    else {
                        if (ReadKey == 11)
                            TriggerKey = state.Gamepad.bLeftTrigger;
                        if (ReadKey == 12)
                            TriggerKey = state.Gamepad.bRightTrigger;
                        if (TriggerKey != 0)
                            GamepadKeyPress = 1;
                        else
                            GamepadKeyPress = 0;
                    }
                }
                else
                    GamepadKeyPress = 0;

                TimeTick = sqrt(Acceleration / CoefficientOfX);

                if (TimeTick >= TimeToMaxAcceleration) // TimeToMaxAcceleration
                    TimeTick = TimeToMaxAcceleration - 0.016f; //TimeToMaxAcceleration - 0.016f

                TimeTick += 0.016f; // The program is approximately 60 frames per second so the time will increase by 0,016 seconds each time.

                Acceleration = CoefficientOfX * TimeTick * TimeTick; // y = ax^2 -- MaxAcceleration = CoefficientOfX * Time^2

                if (TeamBlastBar > 5.0f && (GetAsyncKeyState(KeyboardButtonKey) || GamepadKeyPress)) {
                    WriteProcessMemory(HandleSonicHeroes, (PBYTE*)AccelerationAdress, &Acceleration, sizeof(float), 0);

                    TeamBlastBar -= 0.1f;
                    WriteProcessMemory(HandleSonicHeroes, (PBYTE*)0x009DD72C, &TeamBlastBar, sizeof(float), 0);
                    for (UINT8 i = 0; i < 3; i++)
                        if (Aura[i] != 4 && Aura[i] != 5) {
                            if ((Aura[i] % 16) == 0)
                                Aura[i] = Aura[i] + 0x4;
                            if (Aura[i] > 0 && Aura[i] < 4)
                                Aura[i] = 0x4;
                            WriteProcessMemory(HandleSonicHeroes, (PBYTE*)AuraAddress[i], &Aura[i], sizeof(UINT8), 0);
                        }
                }
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


#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <filesystem>
#include "json.hpp"

#define Velocity_Offset 0xDC
#define Character_Base_Offset 0x8F

bool PressedKey() {

    static uint8_t* isPressedCKey = (uint8_t*)0x00A2FA36;
    static uint8_t* isPressedControllerRightButton = (uint8_t*)0x00A2FB3A;

    if (*isPressedCKey || *isPressedControllerRightButton) return true;

    return false;
}

DWORD WINAPI MainCore(HMODULE hModule) {
    float_t MaximumVelocity = 15.0f;
    float_t InHowManySecond = 2.0f;
    float_t Constant = 5.0f;
    bool BoostForAll = true;
    bool LimitedBoost = false;

    char* Reloaded2EnvVal = std::getenv("RELOADEDIIMODS");
    std::filesystem::path FullPath = "C:\\Reloaded-II\\User\\Mods\\sonicheroes.mechanic.boostforsh\\Config.json";
    if (Reloaded2EnvVal != nullptr) {
        std::filesystem::path ReloadedModsPath = Reloaded2EnvVal;
        FullPath = ReloadedModsPath.parent_path() / "User\\Mods\\sonicheroes.mechanic.boostforsh\\Config.json";
    }

    std::ifstream ConfigFile(FullPath);
    if (ConfigFile.is_open()) {
        try {
            nlohmann::json ConfigData;
            ConfigFile >> ConfigData;

            MaximumVelocity = ConfigData.value("Maximum_Velocity", 15.0f);
            InHowManySecond = ConfigData.value("In_How_Many_Second", 2.0f);
            BoostForAll = ConfigData.value("Boost_For_All", true);
            LimitedBoost = ConfigData.value("Limited_Boost", false);

            if (InHowManySecond < 0.01f) InHowManySecond = 0.01f;
        }
        catch (const nlohmann::json::parse_error&) {
        }

        ConfigFile.close();
    }

    Constant = MaximumVelocity / (InHowManySecond * InHowManySecond);
    const float_t AddConstant = Constant * 0.000256f;
    const float_t MultiplicationConstant = sqrtf(Constant) * 0.032f;

    bool BoostLock = false;
    void** CharacterAddresses = (void**)(0x9CE820);
    byte* TeamBlastState = (byte*)0x9DD73C;
    uint8_t* GameState = (uint8_t*)0x8D66F0;
    uint8_t* CharacterBase = nullptr;
    float_t* Velocity = nullptr;
    float_t* TeamBlastBar = (float_t*)0x9DD72C;

    while (*CharacterAddresses == nullptr) Sleep(100);
    
    while (true) {

        if (*GameState != 5 || !PressedKey()) {
            Sleep(16);
            continue;
        }

        Velocity = reinterpret_cast<float_t*>(reinterpret_cast<uintptr_t>(*CharacterAddresses) + Velocity_Offset);

        if (*Velocity <= 0.0f || *Velocity >= MaximumVelocity) {
            Sleep(16);
            continue;
        }

        // Limited Boost and BoostLock Control
        if (LimitedBoost) {

            if (*TeamBlastBar < 0.1f) BoostLock = true;
            else BoostLock = false;

            if (!BoostLock) *TeamBlastBar -= 0.1f;

            if (*TeamBlastState == 1) *TeamBlastState = 0;
        }

        if (BoostLock) {
            Sleep(16);
            continue;
        }

        // Boost For All
        if (BoostForAll) *Velocity += AddConstant + sqrtf(*Velocity) * MultiplicationConstant;

        // Boost For Speed
        else {
            CharacterBase = reinterpret_cast<uint8_t*>(reinterpret_cast<uintptr_t>(*CharacterAddresses) + Character_Base_Offset);

            if ((*CharacterBase) % 3 == 0 && !BoostLock) {
                *Velocity += AddConstant + sqrtf(*Velocity) * MultiplicationConstant;
            }
        }

        // Orginal Velocity Function:
        // *Velocity = Constant * (sqrtf(*Velocity / Constant) + 0.0016) * (sqrtf(*Velocity / Constant) + 0.0016)

        Sleep(16);

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
    {
        HANDLE hThread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainCore, hModule, 0, nullptr);
        if (hThread != nullptr) CloseHandle(hThread);
        break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
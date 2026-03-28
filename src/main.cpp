#include <windows.h>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <cmath>

constexpr uint8_t VelocityOffset = 0xDC;
constexpr uint8_t CharacterSoulOffset = 0x8F;

bool isKeyPressed() {
    volatile uint8_t* CKey = reinterpret_cast<volatile uint8_t*>(0x00A2FA36);
    volatile uint8_t* ControllerRightButton = reinterpret_cast<volatile uint8_t*>(0x00A2FB3A);
    return (*CKey > 0 || *ControllerRightButton > 0);
}

DWORD WINAPI MainCore(LPVOID lpParam) {
    HMODULE myHModule = reinterpret_cast<HMODULE>(lpParam);

    char* Reloaded2EnvVal = std::getenv("RELOADEDIIMODS");
    std::filesystem::path Reloaded2Path = "C:/Reloaded-II";
    if (Reloaded2EnvVal != nullptr) Reloaded2Path = Reloaded2EnvVal;

    float MaximumVelocity = 15.0f, InHowManySecond = 2.0f;
    bool BoostForAll = true, LimitedBoost = false;
    std::ifstream ConfigFile(Reloaded2Path.parent_path() / "User/Mods/sonicheroes.mechanic.boostforsh/Config.json");
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

    const uint32_t SonicHeroesBaseAddress = reinterpret_cast<uint32_t>(GetModuleHandle(NULL));
    const uint32_t CharacterPtr = 0x005CE820 + SonicHeroesBaseAddress;
    while (*reinterpret_cast<volatile void**>(CharacterPtr) == nullptr) Sleep(100);

    const float Constant = MaximumVelocity / (InHowManySecond * InHowManySecond);
    const float AddConstant = Constant * 0.000256f;
    const float MultiplicationConstant = sqrtf(Constant) * 0.032f;

    while (true) {
        volatile uint8_t* GameState = reinterpret_cast<volatile uint8_t*>(0x004D66F0 + SonicHeroesBaseAddress);
        if (*GameState != 5 || !isKeyPressed()) {
            Sleep(16);
            continue;
        }

        volatile float* Velocity = reinterpret_cast<volatile float*>(*reinterpret_cast<volatile uint32_t*>(CharacterPtr) + VelocityOffset);
        if (*Velocity <= 0.0f || *Velocity >= MaximumVelocity) {
            Sleep(16);
            continue;
        }

        if (!BoostForAll) {
            volatile uint8_t* Soul =
                reinterpret_cast<volatile uint8_t*>(
                    *reinterpret_cast<volatile uint32_t*>(CharacterPtr) + CharacterSoulOffset);
            if (*Soul % 3 != 0) {
                Sleep(16);
                continue;
            }
        }
        
        if (LimitedBoost) {
            volatile float* TeamBlastBar = reinterpret_cast<volatile float*>(0x005DD72C+ SonicHeroesBaseAddress);
            bool BoostLock = (*TeamBlastBar < 0.1f);
            if (BoostLock) {
                Sleep(16);
                continue;
            }
            *TeamBlastBar -= 0.1f;

            volatile uint8_t* TeamBlastState = reinterpret_cast<volatile uint8_t*>(0x005DD73C+ SonicHeroesBaseAddress);
            if (*TeamBlastState == 1) *TeamBlastState = 0;

        }

        *Velocity += AddConstant + sqrtf(*Velocity) * MultiplicationConstant;

        Sleep(16);
    }

    FreeLibraryAndExitThread(myHModule, 0);
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
            DisableThreadLibraryCalls(hModule);
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

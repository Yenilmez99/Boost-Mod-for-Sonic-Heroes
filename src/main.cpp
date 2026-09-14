#include "helper.h"
#include "MinHook.h"

#include <synchapi.h>
#include <windows.h>
#include <wtypesbase.h>

// Orginal Timer Function (Timer Works on stage so we need to hook it)
typedef void(__cdecl* OrginalTimer)();
OrginalTimer OrginalTimerFunction = nullptr;

typedef void(__cdecl* DashRing)(int characterNo);
DashRing SpawnDashRing = (DashRing)0x00630A10;

unsigned char FrameCounter = 0;
float Constant = 0;
float AddConstant = 0;
float MultiplicationConstant = 0;
inline Settings ConfigSettings;
void BoostLogic() {
    
    if (!isKeyPressed(ConfigSettings.Boost_Controller_Key)) return;

    volatile uintptr_t& RoleBase = *reinterpret_cast<volatile uintptr_t*>(0x00A4C268);
    volatile uint8_t& Role = *reinterpret_cast<uint8_t*>(RoleBase + 0x3B);

    volatile uintptr_t& CharacterBase = reinterpret_cast<volatile uintptr_t*>(0x00A4B1B0)[Role];
    volatile float& Velocity = *reinterpret_cast<volatile float*>(CharacterBase + 0x108);

    if (Velocity <= 0.0f || Velocity >= ConfigSettings.Maximum_Velocity) return;

    if (!ConfigSettings.Boost_For_All && Role != 0) return;

    if (ConfigSettings.Limited_Boost) {
        volatile float& TeamBlastBar = *reinterpret_cast<volatile float*>(0x009DD72C);
        if (TeamBlastBar <= 0.0f) {
            TeamBlastBar = 0.0f;
            return;
        }

        TeamBlastBar -= 0.1f;

        volatile uint8_t& TeamBlastState = *reinterpret_cast<volatile uint8_t*>(0x009DD73C);
        if (TeamBlastState == 1) TeamBlastState = 0;

    }

    Velocity += AddConstant + sqrtf(Velocity) * MultiplicationConstant;

    if (!ConfigSettings.Boost_Rings) return;
    FrameCounter++;
    if (FrameCounter >= ConfigSettings.Boost_Rings_Frequency) {
        FrameCounter = 0;

        for (int i = 0; i < ConfigSettings.Boost_Rings_Color_Intensity; i++) {
            SpawnDashRing(0); // for speed;
            if (ConfigSettings.Boost_For_All) {
                SpawnDashRing(1); // for fly;
                SpawnDashRing(2); // for power;
            }

        }
    }

    return;
}

__declspec(naked) void HookTimer() {
    __asm {
        pushad
        pushfd

        call BoostLogic

        popfd
        popad

        jmp [OrginalTimerFunction]
    }
}

DWORD WINAPI MainCore(LPVOID lpParam) {
    HMODULE myHModule = reinterpret_cast<HMODULE>(lpParam);

    GetSettings(myHModule, ConfigSettings);

    Constant = ConfigSettings.Maximum_Velocity / (ConfigSettings.In_How_Many_Second * ConfigSettings.In_How_Many_Second);
    AddConstant = Constant * 0.000256f; // 60fps = 0.016 microsecond so fps^2 = 0.000256
    MultiplicationConstant = sqrtf(Constant) * 0.032f;

    MH_STATUS mhStatus = MH_Initialize();
    if (mhStatus != MH_OK && mhStatus != MH_ERROR_ALREADY_INITIALIZED) {
        return 0;
    }

    MH_CreateHook(reinterpret_cast<LPVOID>(0x00423F20), reinterpret_cast<LPVOID>(&HookTimer), reinterpret_cast<LPVOID*>(&OrginalTimerFunction));

    // Active to Hooks
    MH_EnableHook(MH_ALL_HOOKS);

    // Stuck Loop
    while (!(GetAsyncKeyState(VK_F1)&0x1)) {
        Sleep(100);
    }

    MH_DisableHook(MH_ALL_HOOKS);
    MH_Uninitialize();

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call)
    {
	case DLL_PROCESS_ATTACH:
    {
        HANDLE hThread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainCore, hModule, 0, nullptr);
        if (hThread != nullptr) CloseHandle(hThread);
        break;
    }
	case DLL_PROCESS_DETACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}
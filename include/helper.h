#pragma once
#include <fstream>
#include <windows.h>
#include <filesystem>

#include "nlohmann/json.hpp"

#include <dinput.h>
#include <unordered_map>

struct Settings {
    int Boost_Controller_Key = 5;
    int Boost_Rings_Color_Intensity = 5;
    int Boost_Rings_Frequency = 5;
    float Maximum_Velocity = 15.0f;
    float In_How_Many_Second = 2.0f;
    bool Boost_For_All = true;
    bool Limited_Boost = false;
    bool Boost_Rings = false; // Boost Ring frequency
};

bool inline isKeyPressed(const int& Boost_Controller_Key) {
    volatile uint8_t& CKey = *reinterpret_cast<volatile uint8_t*>(0x00A2FA36);
    volatile DIJOYSTATE2& dInputJoystickState = *reinterpret_cast<volatile DIJOYSTATE2*>(0x00A2FB08);
    return (CKey > 0 || dInputJoystickState.rgbButtons[Boost_Controller_Key] > 0);
}

inline void GetSettings(HMODULE& dllHModule, Settings& settings) {
    char dllPathStr[MAX_PATH];
    
    // get dlls full path (example: C:\Mods\MyMod\mydll.dll)
    GetModuleFileNameA(dllHModule, dllPathStr, MAX_PATH);

    // change it and find config files path
    std::filesystem::path DllPath(dllPathStr);
    std::filesystem::path ConfigFilePath = DllPath.parent_path().parent_path().parent_path() / "User/Mods/sonicheroes.mechanic.boostforsh/Config.json";

    std::ifstream ConfigFile(ConfigFilePath);

    if (ConfigFile.is_open()) {
        try {
            nlohmann::json ConfigData;
            ConfigFile >> ConfigData;

            settings.Boost_Rings_Color_Intensity = ConfigData.value("Boost_Rings_Color_Intensity", 5);
            settings.Boost_Rings_Frequency = ConfigData.value("Boost_Rings_Frequency", 5);
            settings.Maximum_Velocity = ConfigData.value("Maximum_Velocity", 15.0f);
            settings.In_How_Many_Second = ConfigData.value("In_How_Many_Second", 2.0f);
            settings.Boost_For_All = ConfigData.value("Boost_For_All", true);
            settings.Limited_Boost = ConfigData.value("Limited_Boost", false);
            settings.Boost_Rings = ConfigData.value("Boost_Rings", false);

            std::unordered_map<std::string, int> controllerKeyMap = {
            {"XBOX360_Controller_Key_A", 0},
            {"XBOX360_Controller_Key_B", 1},
            {"XBOX360_Controller_Key_X", 2},
            {"XBOX360_Controller_Key_Y", 3},
            {"XBOX360_Controller_Key_LB", 4},
            {"XBOX360_Controller_Key_RB", 5},
            {"XBOX360_Controller_Key_BACK", 6},
            {"XBOX360_Controller_Key_START", 7},
            {"XBOX360_Controller_Key_LS", 8},
            {"XBOX360_Controller_Key_RS", 9}
            };

            std::string keyStr = ConfigData.value("Boost_Controller_Key", "XBOX360_Controller_Key_RB");
            if (controllerKeyMap.find(keyStr) != controllerKeyMap.end()) {
            settings.Boost_Controller_Key = controllerKeyMap[keyStr];
            }

        }
        catch (const nlohmann::json::exception&) {
        }

        ConfigFile.close();
    }
}
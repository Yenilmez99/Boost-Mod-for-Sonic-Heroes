#pragma once
#include <fstream>
#include <windows.h>
#include <filesystem>

#include "nlohmann/json.hpp"

struct Settings {
    int Boost_Rings_Color_Intensity = 5;
    int Boost_Rings_Frequency = 5;
    float Maximum_Velocity = 15.0f;
    float In_How_Many_Second = 2.0f;
    bool Boost_For_All = true;
    bool Limited_Boost = false;
    bool Boost_Rings = false; // Boost Ring frequency
};

bool inline isKeyPressed() {
    volatile uint8_t* CKey = reinterpret_cast<volatile uint8_t*>(0x00A2FA36);
    volatile uint8_t* ControllerRightButton = reinterpret_cast<volatile uint8_t*>(0x00A2FB3D);
    return (*CKey > 0 || *ControllerRightButton > 0);
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

        }
        catch (const nlohmann::json::exception&) {
        }

        ConfigFile.close();
    }
}
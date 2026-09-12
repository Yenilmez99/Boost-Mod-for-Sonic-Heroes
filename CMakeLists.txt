cmake_minimum_required(VERSION 3.20)

project(SH_boost_workspace VERSION 4.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)

FetchContent_Declare(
    minhook
    GIT_REPOSITORY https://github.com/TsudaKageyu/minhook.git
    GIT_TAG v1.3.4
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(minhook)

add_library(SH_boost SHARED src/dllmain.cpp)
set_target_properties(SH_boost PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../sonicheroes.mechanic.boostforsh"
)

target_link_libraries(SH_boost PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(SH_boost PRIVATE minhook)

set_target_properties(SH_boost PROPERTIES OUTPUT_NAME "SH_boost")

target_include_directories(SH_boost PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)
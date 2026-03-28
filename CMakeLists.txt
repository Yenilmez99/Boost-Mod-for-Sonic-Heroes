cmake_minimum_required(VERSION 3.24)

project("SonicHeroesBoostMod" C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG        v3.11.3
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(json)

set(SOURCE_FILES "src/main.cpp")
set(OUTPUT_FOLDER "../sonicheroes.mechanic.boostforsh")

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    PREFIX ""
    OUTPUT_NAME "SonicHeroesBoostMod"
    LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_FOLDER}"
    RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_FOLDER}"
)

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

if(MINGW)
target_link_options(${PROJECT_NAME} PRIVATE "-static-libgcc" "-static-libstdc++" "-static")
endif()

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")

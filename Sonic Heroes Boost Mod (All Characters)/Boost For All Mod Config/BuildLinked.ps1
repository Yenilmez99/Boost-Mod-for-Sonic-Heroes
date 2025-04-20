# Set Working Directory
Split-Path $MyInvocation.MyCommand.Path | Push-Location
[Environment]::CurrentDirectory = $PWD

Remove-Item "$env:RELOADEDIIMODS/Boost_For_All_Mod_Config/*" -Force -Recurse
dotnet publish "./Boost_For_All_Mod_Config.csproj" -c Release -o "$env:RELOADEDIIMODS/Boost_For_All_Mod_Config" /p:OutputPath="./bin/Release" /p:ReloadedILLink="true"

# Restore Working Directory
Pop-Location
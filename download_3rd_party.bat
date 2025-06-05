@echo off

echo "---------- Download PopcornFX Unity plugin 3rd party libraries ----------"

setlocal
set OPEN_SOURCE_UNITY_REPO_PATH=%~dp0

bitsadmin /reset
bitsadmin /create third_party_download
bitsadmin /addfile third_party_download http://downloads.popcornfx.com/Plugins/ExternalLibs/ExternalLibs_UnityStore_2.21.4-23725_android_android64_arm64_ios64_x32_x64_android_ios_linux_macosx_vs2019.zip "%OPEN_SOURCE_UNITY_REPO_PATH%ExternalLibs.zip"
bitsadmin /setpriority third_party_download "FOREGROUND"
bitsadmin /resume third_party_download

:WAIT_FOR_DOWNLOAD_LOOP_START
    call bitsadmin /info third_party_download /verbose | find "STATE: TRANSFERRED"
    if %ERRORLEVEL% equ 0 goto WAIT_FOR_DOWNLOAD_LOOP_END
    call bitsadmin /RawReturn /GetBytesTransferred third_party_download
    echo Bytes transferred
    timeout 2 > nul
    goto WAIT_FOR_DOWNLOAD_LOOP_START
:WAIT_FOR_DOWNLOAD_LOOP_END

bitsadmin /complete third_party_download

echo "---------- Download complete ----------"
echo "---------- Cleaning %OPEN_SOURCE_UNITY_REPO_PATH%ExternalLibs ----------"

rmdir /s /q "%OPEN_SOURCE_UNITY_REPO_PATH%ExternalLibs"

echo "---------- Unzipping %OPEN_SOURCE_UNITY_REPO_PATH%ExternalLibs.zip ----------"

powershell.exe -nologo -noprofile -command "& { Add-Type -A 'System.IO.Compression.FileSystem'; [IO.Compression.ZipFile]::ExtractToDirectory('%OPEN_SOURCE_UNITY_REPO_PATH%ExternalLibs.zip', '%OPEN_SOURCE_UNITY_REPO_PATH%'); }"

echo "---------- Removing %OPEN_SOURCE_UNITY_REPO_PATH%ExternalLibs.zip ----------"

del /f "%OPEN_SOURCE_UNITY_REPO_PATH%ExternalLibs.zip"

echo "---------- Finished ----------"

endlocal

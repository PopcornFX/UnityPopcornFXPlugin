#!/bin/bash

SCRIPT_PATH=$(readlink -f "$0")
OPEN_SOURCE_UNITY_REPO_PATH=$(dirname "$SCRIPT_PATH")

rm -rf "$OPEN_SOURCE_UNITY_REPO_PATH/ExternalLibs"
wget "http://downloads.popcornfx.com/Plugins/ExternalLibs/ExternalLibs_UnityStore_2.20.13-26695_arm64_x64_ios64_android_x32_android64_macosx_ios_vs2019_linux_android.zip" -O "$OPEN_SOURCE_UNITY_REPO_PATH/ExternalLibs.zip"
unzip "$OPEN_SOURCE_UNITY_REPO_PATH/ExternalLibs.zip"
rm -f "$OPEN_SOURCE_UNITY_REPO_PATH/ExternalLibs.zip"

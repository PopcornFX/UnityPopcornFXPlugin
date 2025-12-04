#!/bin/bash

SCRIPT_PATH=$(readlink -f "$0")
OPEN_SOURCE_UNITY_REPO_PATH=$(dirname "$SCRIPT_PATH")

rm -rf "$OPEN_SOURCE_UNITY_REPO_PATH/ExternalLibs"
wget "http://downloads.popcornfx.com/Plugins/ExternalLibs/ExternalLibs_UnityStore_2.23.1-25200_x64_android64_ios64_android_arm64_linux_macosx_ios_android_vs2019.zip" -O "$OPEN_SOURCE_UNITY_REPO_PATH/ExternalLibs.zip"
unzip "$OPEN_SOURCE_UNITY_REPO_PATH/ExternalLibs.zip"
rm -f "$OPEN_SOURCE_UNITY_REPO_PATH/ExternalLibs.zip"

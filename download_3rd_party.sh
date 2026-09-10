#!/bin/bash

SCRIPT_PATH=$(readlink -f "$0")
OPEN_SOURCE_UNITY_REPO_PATH=$(dirname "$SCRIPT_PATH")

rm -rf "$OPEN_SOURCE_UNITY_REPO_PATH/ExternalLibs"
wget "http://downloads.popcornfx.com/Plugins/ExternalLibs/ExternalLibs_UnityStore_2.24.6-27934_x64_android64_ios64_android_arm64_android_vs2019_ios_macosx_linux.zip" -O "$OPEN_SOURCE_UNITY_REPO_PATH/ExternalLibs.zip"
unzip "$OPEN_SOURCE_UNITY_REPO_PATH/ExternalLibs.zip"
rm -f "$OPEN_SOURCE_UNITY_REPO_PATH/ExternalLibs.zip"

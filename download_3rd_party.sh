#!/bin/bash

SCRIPT_PATH=$(readlink -f "$0")
OPEN_SOURCE_UNITY_REPO_PATH=$(dirname "$SCRIPT_PATH")

rm -rf "$OPEN_SOURCE_UNITY_REPO_PATH/ExternalLibs"
wget "http://downloads.popcornfx.com/SDK/b32d93a3/ExternalLibs_CommunityLicense_Unity_2.12.4-13167_android_android64_x64_android_linux_vs2019.zip" -o "$OPEN_SOURCE_UNITY_REPO_PATH/ExternalLibs.zip"
unzip "$OPEN_SOURCE_UNITY_REPO_PATH/ExternalLibs.zip"
rm -f "$OPEN_SOURCE_UNITY_REPO_PATH/ExternalLibs.zip"

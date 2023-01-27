//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
#import "UnityAppController.h"
#include "Unity/IUnityGraphics.h"

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API PKIOSUnityPluginLoad(IUnityInterfaces* unityInterfaces);
extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API PKIOSUnityPluginUnload();

@interface PKFxPluginController : UnityAppController
{
}
- (void)shouldAttachRenderDelegate;
@end
@implementation PKFxPluginController
- (void)shouldAttachRenderDelegate
{
	// unlike desktops where plugin dynamic library is automatically loaded and registered
	// we need to do that manually on iOS
	UnityRegisterRenderingPluginV5(&PKIOSUnityPluginLoad, &PKIOSUnityPluginUnload);
}

@end
IMPL_APP_CONTROLLER_SUBCLASS(PKFxPluginController);

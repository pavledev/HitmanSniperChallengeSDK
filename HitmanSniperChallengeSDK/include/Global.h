#pragma once

#include <unordered_map>
#include <string>
#include <Windows.h>

#include "Common.h"

class DebugConsole;
class ZRenderManager;
class ZLevelManager;
class ZGraphicsSettingsManager;
class ZMemoryManager;
class PipeServer;
class SharedMemoryServer;
class EngineController;
class ZGameTimeManager;
class ZInputDeviceManagerWindows;
class ZInputActionManager;
class ZHitman5Module;
class ZGameLoopManager;
class ZGameWideUI;
class ZHUDManager;
class ZScaleformManager;
class ZInputAction;
class ZCollisionManager;
class ZTypeRegistry;
class ZContentKitManager;
class ZResourceManager;
class ZActorManager;
class LocalResourceIDsResolver;
class ZCheckPointManager;
class ZEntityType;
class ZVariantRef;
class ZHM5ActionManager;
class ZEntityManager;
class ZString;
class ZMorphemeMediator;

extern HitmanSniperChallengeSDK_API uintptr_t BaseAddress;
extern HitmanSniperChallengeSDK_API ZRenderManager* RenderManager;
extern HitmanSniperChallengeSDK_API ZLevelManager* LevelManager;
extern HitmanSniperChallengeSDK_API ZGraphicsSettingsManager* GraphicsSettingsManager;
extern HitmanSniperChallengeSDK_API ZMemoryManager* MemoryManager;
extern HitmanSniperChallengeSDK_API ZGameTimeManager* GameTimeManager;
extern HitmanSniperChallengeSDK_API ZInputDeviceManagerWindows* InputDeviceManager;
extern HitmanSniperChallengeSDK_API ZInputActionManager* InputActionManager;
extern HitmanSniperChallengeSDK_API ZHitman5Module* Hitman5Module;
extern HitmanSniperChallengeSDK_API ZGameLoopManager* GameLoopManager;
extern HitmanSniperChallengeSDK_API ZGameWideUI* GameWideUI;
extern HitmanSniperChallengeSDK_API ZHUDManager* HUDManager;
extern HitmanSniperChallengeSDK_API ZScaleformManager* ScaleformManager;
extern HitmanSniperChallengeSDK_API ZInputAction* HM5InputControl;
extern HitmanSniperChallengeSDK_API ZCollisionManager* CollisionManager;
extern HitmanSniperChallengeSDK_API ZTypeRegistry** TypeRegistry;
extern HitmanSniperChallengeSDK_API ZContentKitManager* ContentKitManager;
extern HitmanSniperChallengeSDK_API ZResourceManager* ResourceManager;
extern HitmanSniperChallengeSDK_API ZActorManager* ActorManager;
extern HitmanSniperChallengeSDK_API LocalResourceIDsResolver** LocalResourceIDsResolverSingleton;
extern HitmanSniperChallengeSDK_API ZCheckPointManager* CheckPointManager;
extern HitmanSniperChallengeSDK_API ZHM5ActionManager* HM5ActionManager;
extern HitmanSniperChallengeSDK_API ZEntityManager* EntityManager;
extern HitmanSniperChallengeSDK_API ZMorphemeMediator* MorphemeMediator;
extern HitmanSniperChallengeSDK_API bool IsEngineInitialized;
extern HitmanSniperChallengeSDK_API void* ZTemplateEntityFactoryVFTbl;
extern HitmanSniperChallengeSDK_API void* ZTemplateEntityBlueprintFactoryVFTbl;
extern HitmanSniperChallengeSDK_API void* ZAspectEntityFactoryVFTbl;
extern HitmanSniperChallengeSDK_API void* ZAspectEntityBlueprintFactoryVFTbl;

extern WNDPROC originalWndProc;

HitmanSniperChallengeSDK_API bool SetPropertyValue(ZEntityType** pEntity, unsigned int nPropertyID, const ZVariantRef& value, bool bInvokeChangeHandlers = true);
HitmanSniperChallengeSDK_API void SignalInputPin(ZEntityType** pEntity, unsigned int nPinID, const ZVariantRef& data);
HitmanSniperChallengeSDK_API void SignalOutputPin(ZEntityType** pEntity, unsigned int nPinID, const ZVariantRef& data);
HitmanSniperChallengeSDK_API bool GetApplicationOptionBool(const ZString& sName, bool bDefault);

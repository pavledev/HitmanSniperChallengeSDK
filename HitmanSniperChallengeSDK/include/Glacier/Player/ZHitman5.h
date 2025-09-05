#pragma once

#include "../IComponentInterface.h"
#include "../ZHM5BaseCharacter.h"
#include "../Camera/ZHM5MainCamera.h"
#include "../Input/ZHM5InputControl.h"
#include "../Physics/ICharacterCollision.h"
#include "ZHM5FocusController.h"
#include "../Resource/ZRuntimeResourceID.h"
#include "ZHM5BaseInventory.h"

class ZHM5Action;
struct float4;
struct SMatrix;
enum eMorphemeRequests;
struct SGameUpdateEvent;

class HitmanSniperChallengeSDK_API IFutureCameraState : public IComponentInterface
{
public:
	struct SCameraState;

	virtual ~IFutureCameraState() = default;
	virtual SCameraState GetFutureCameraState() = 0;
	virtual SCameraState GetCurrentCameraState() = 0;
	virtual void DisableCameraControl() = 0;
	virtual void EnableCameraControl() = 0;
};

class HitmanSniperChallengeSDK_API IHM5ActionEntityListener : public IComponentInterface
{
public:
	virtual ~IHM5ActionEntityListener() = default;
	virtual bool GetActionAABB(const ZHM5Action* pAction, float4& vMin, float4& vMax) const = 0;
	virtual bool IsActionValid(ZHM5Action* action, const TEntityRef<ZHM5BaseCharacter>& entityRef) = 0;
	virtual bool DisplayFarFeedback(ZHM5Action* pAction, const TEntityRef<ZHM5BaseCharacter>& pOperator) = 0;
	virtual bool ActivateAction(ZHM5Action* pAction, const TEntityRef<ZHM5BaseCharacter>& pOperator) = 0;
};

class HitmanSniperChallengeSDK_API ZHitman5 : public ZHM5BaseCharacter, public IFutureCameraState, public ICharacterCollision, public IHM5ActionEntityListener
{
public:
	ZHM5MainCamera* GetMainCamera() const;
	ZHM5InputControl* GetInputControl() const;
	ZHM5BaseInventory* GetBaseInventory() const;
	ZHM5FocusController* GetFocusController() const;
	void EquipOutfitResource(ZRuntimeResourceID ridNewOutfitLibrary, const TEntityRef<IHM5Outfit> Outfit, bool bPlaceClothBundle, bool bForce);

private:
	PAD(0x754);
	ZHM5InputControl* m_pInputControl; //0x970
	PAD(0x10);
	ZHM5BaseInventory* m_pBaseInventory; //0x984
	PAD(0x10);
	ZHM5FocusController* m_pFocusController; //0x998
	PAD(0x284);
	TEntityRef<ZHM5MainCamera> m_rMainCamera; //0xC20
};

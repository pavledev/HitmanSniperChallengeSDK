#pragma once

#include "Entity/ZEntityImpl.h"
#include "Templates/TEntityRef.h"
#include "Render/ZGeomEntity.h"

#include <Common.h>

struct SHitInfo;
class ZSpatialEntity;
class ISequenceEntity;
struct SMatrix43;
class ZEvent0;
struct SCutSequenceData;
class ZRayQueryInput;
class ZRayQueryOutput;
class ICharacterController;
class IHM5Item;
class ZLinkedEntity;
class IHM5Outfit;
class IPhysicsObject;
class IMorphemeEntity;
struct SAnimSample;
class IEventConsumerCollection;
class ZHM5Ragdoll;
struct float4;
struct SMatrix;

class HitmanSniperChallengeSDK_API IHM5BaseCharacter : public IComponentInterface
{
public:
	virtual ~IHM5BaseCharacter() = default;
};

class HitmanSniperChallengeSDK_API IBaseCharacter : public IComponentInterface
{
public:
	virtual ~IBaseCharacter() = default;
};

class HitmanSniperChallengeSDK_API IMorphemeCutSequenceAnimatable
{
public:
	virtual void CutSequenceStarted(ISequenceEntity* sequenceEntity, const ZString& string, float param3) = 0;
	virtual void CutSequenceUpdate(ISequenceEntity* sequenceEntity, ZString string, float param3) = 0;
	virtual SMatrix43 CutSequenceGetWorldTransform() = 0;
	virtual void CutSequenceSetWorldTransform(const SMatrix43& matrix43) = 0;
	virtual SMatrix43 CutSequenceGetParentTransform() = 0;
	virtual void CutSequenceSetParentTransform(const SMatrix43& matrix43) = 0;
	virtual void CutSequenceEnded(ISequenceEntity* sequenceEntity, ZString string) = 0;
	virtual ZEvent0& GetAnimatableChangedEvent() = 0;
	virtual void AbortCutSequence() = 0;
	virtual bool SetCutSequenceData(ISequenceEntity* sequenceEntity, const SCutSequenceData& cutSequenceData, const ZString& string, float param4) = 0;
	virtual ~IMorphemeCutSequenceAnimatable() = default;
};

class HitmanSniperChallengeSDK_API IBoneCollidable
{
public:
	virtual bool CheckLineCollision(const ZRayQueryInput& rayQueryInput, ZRayQueryOutput* rayQueryOutput) const = 0;
	virtual ICharacterController* GetCharacterController() const = 0;
	virtual ~IBoneCollidable() = default;
};

class HitmanSniperChallengeSDK_API alignas(16) ZHM5BaseCharacter : public ZEntityImpl, public IHM5BaseCharacter, public IBaseCharacter, public IMorphemeCutSequenceAnimatable, public IBoneCollidable
{
public:
	virtual void Unk16() = 0;
	virtual void Unk17() = 0;
	virtual void Unk18() = 0;
	virtual void Unk19() = 0;
	virtual void Unk20() = 0;
	virtual void Unk21() = 0;
	virtual void Unk22() = 0;
	virtual void Unk23() = 0;
	virtual void Unk24() = 0;
	virtual void Unk25() = 0;
	virtual void Unk26() = 0;
	virtual void Unk27() = 0;
	virtual TEntityRef<ZGeomEntity> GetGeomEntity() const = 0;
	virtual ZGeomEntity* GetGeomEntityPtr() const = 0;
	virtual ZSpatialEntity* GetSpatialEntityPtr() const = 0;
	virtual TEntityRef<ZSpatialEntity> GetSpatialEntity() const = 0;

private:
	PAD(0x1F8);
};

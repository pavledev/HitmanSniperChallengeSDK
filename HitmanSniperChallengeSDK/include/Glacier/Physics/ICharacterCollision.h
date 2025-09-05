#pragma once

class HitmanSniperChallengeSDK_API ICharacterCollision : public IComponentInterface
{
public:
	virtual ~ICharacterCollision() = default;
	virtual SMatrix GetCollisionCheckedMatPos(const SMatrix& matrix) = 0;
};

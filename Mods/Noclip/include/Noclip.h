#pragma once

#include <Glacier/SGameUpdateEvent.h>
#include <Glacier/Input/ZInputAction.h>

#include <ModInterface.h>

class Noclip : public ModInterface
{
public:
	Noclip();
	~Noclip();

	void OnEngineInitialized() override;
	void OnDrawMenu() override;

private:
	void OnFrameUpdate(const SGameUpdateEvent& updateEvent);

	bool areBindingsAdded;
	bool isNoclipEnabled;
	ZInputAction toggleNoclipAction;
	float4 playerPosition;
};

DECLARE_MOD(Noclip)

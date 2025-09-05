#pragma once

#include <Common.h>

#include "../ZDelegate.h"

class ZResourceStub;
class ZRuntimeResourceID;

class HitmanSniperChallengeSDK_API ZResourcePtr
{
public:
	ZResourcePtr() = default;
	ZResourcePtr(const ZResourcePtr& rhs);
	~ZResourcePtr();
	ZResourceStub* GetResourceStub() const;
	void* GetRawPointer() const;
	bool IsReady() const;
	bool Exists() const;
	bool Failed() const;
	void AddStatusChangedListener(const ZDelegate<void __cdecl(ZRuntimeResourceID const&)>& d);
	void RemoveStatusChangedListener(const ZDelegate<void __cdecl(ZRuntimeResourceID const&)>& d);
	bool operator==(const ZResourcePtr& rhs) const;

private:
	ZResourceStub* m_pResourceStub;
};

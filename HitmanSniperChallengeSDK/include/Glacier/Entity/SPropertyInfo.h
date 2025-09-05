#pragma once

struct STypeID;

struct SPropertyInfo
{
	STypeID* m_Type;
	unsigned int m_Flags;
	int m_nPropertyOffset;
	unsigned int m_nExtraData;
	void (*m_PropetyGetter)(void* param1, void* param2, unsigned int param3);
	void (*m_PropertySetCallBack)(void* param1, void* param2, unsigned int param3, bool param4);
};

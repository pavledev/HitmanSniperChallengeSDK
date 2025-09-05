#include <Glacier/UI/GFxValue.h>

#include <Function.h>
#include <Global.h>

bool GFxValue::GetBool() const
{
	return Value.BValue;
}

void GFxValue::SetBool(const bool value)
{
	Value.BValue = value;
}

bool GFxValue::GetMember(const char* name, GFxValue* pval) const
{
	return pObjectInterface->GetMember(name, pval, (Type & 0x8F) == 8);
}

bool GFxValue::SetMember(const char* name, const GFxValue& val)
{
	return pObjectInterface->SetMember(name, val, (Type & 0x8F) == 8);
}

bool GFxValue::ObjectInterface::GetMember(const char* name, GFxValue* pval, bool isdobj)
{
	return Function::CallMethodAndReturn<bool, const GFxValue::ObjectInterface*, const char*, GFxValue*, bool>(BaseAddress + 0x896480, this, name, pval, isdobj);
}

bool GFxValue::ObjectInterface::SetMember(const char* name, const GFxValue& value, bool isdobj)
{
	return Function::CallMethodAndReturn<bool, const GFxValue::ObjectInterface*, const char*, const GFxValue&, bool>(BaseAddress + 0x894970, this, name, value, isdobj);
}

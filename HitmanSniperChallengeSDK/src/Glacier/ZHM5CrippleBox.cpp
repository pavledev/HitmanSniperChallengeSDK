#include "Glacier/ZHM5CrippleBox.h"

#include "Global.h"
#include "Function.h"

void ZHM5CrippleBox::SetDataOnHitman(bool bDefaultFlags)
{
	Function::Call<bool>(BaseAddress + 0x351F10, bDefaultFlags);
}

void ZHM5CrippleBox::UpdateFlags()
{
	Function::CallMethod<ZHM5CrippleBox*>(BaseAddress + 0x351A70, this);
}

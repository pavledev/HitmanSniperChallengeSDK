#pragma once

#include <Glacier/Resource/ZRuntimeResourceID.h>
#include <Glacier/STokenID.h>

#include <ModInterface.h>

class Actors : public ModInterface
{
public:
    Actors();
    void OnDrawMenu() override;
    void OnDrawUI(const bool hasFocus) override;
    void OnDraw3D() override;

private:
    bool isOpen;
    int selectedActorIndex;
    bool renderActorNames;
};

DECLARE_MOD(Actors)

#include "pch.h"
#include "GateInteractComponent.h"
#include "BossGate.h"
#include "Input/InputAction.h"

bool GateInteractComponent::Init(int32 Id, const std::string& Name, Ptr<class Actor> Owner)
{
    return ActorComponent::Init(Id, Name, Owner);
}

void GateInteractComponent::Destroy()
{
    _OverlappingGate.reset();

    ActorComponent::Destroy();
}

void GateInteractComponent::HandleInput(Ptr<class InputAction> Action, INPUT_TYPE::eType ButtonEvent)
{
    if (!Action || Action->GetName() != "PORTAL_INTERACT" || ButtonEvent != INPUT_TYPE::DOWN)
    {
        return;
    }

    Ptr<BossGate> CurrentGate = Lock(_OverlappingGate);

    if (!CurrentGate)
    {
        return;
    }

    if (CurrentGate->Interact())
    {
        _OverlappingGate.reset();
    }
}

void GateInteractComponent::SetOverlappingGate(Ptr<class BossGate> GateActor)
{
    if (!GateActor)
    {
        return;
    }

    _OverlappingGate = GateActor;
}

void GateInteractComponent::ClearOverlappingGate(Ptr<class BossGate> GateActor)
{
    Ptr<BossGate> CurrentGate = Lock(_OverlappingGate);

    if (CurrentGate == GateActor)
    {
        _OverlappingGate.reset();
    }
}

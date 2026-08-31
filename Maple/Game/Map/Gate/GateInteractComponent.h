#pragma once
#include "Component/ActorComponent.h"

class GateInteractComponent : public ActorComponent
{
public:
	GateInteractComponent() = default;
	virtual ~GateInteractComponent() = default;
	GateInteractComponent(const GateInteractComponent&) = delete;
	GateInteractComponent(GateInteractComponent&&) = delete;
	GateInteractComponent& operator = (const GateInteractComponent&) = delete;
	GateInteractComponent& operator = (GateInteractComponent&&) = delete;

private:
	Weak<class BossGate> _OverlappingGate;

public:
	virtual bool Init(int32 Id, const std::string& Name, Ptr<class Actor> Owner) override;

	virtual void Destroy() override;

	void HandleInput(Ptr<class InputAction> Action, INPUT_TYPE::eType ButtonEvent);

	void SetOverlappingGate(Ptr<class BossGate> GateActor);

	void ClearOverlappingGate(Ptr<class BossGate> GateActor);
};


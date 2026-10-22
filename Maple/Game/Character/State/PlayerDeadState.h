#pragma once
#include "PlayerState.h"
#include "UI/DeadNotice.h"

class PlayerDeadState : public PlayerState
{
public:
	PlayerDeadState() = default;
	virtual ~PlayerDeadState() = default;

private:
	float _ElapsedTime = 0.f;
	
	Ptr<DeadNotice> _Notice;
	
public:
	virtual void Enter(Ptr<class PlayerComponent> PlayerComponent) override;
	
	virtual void Exit(Ptr<class PlayerComponent> PlayerComponent) override;
	
	virtual Ptr<PlayerState> Tick(Ptr<class PlayerComponent> PlayerComponent, float DeltaTime) override;
	
	virtual Ptr<PlayerState> HandleInput(Ptr<class PlayerComponent> PlayerComponent, Ptr<InputAction> Action, INPUT_TYPE::eType ButtonEvent) override;

	virtual ePlayerAnimationType GetAnimationType() const override;
};


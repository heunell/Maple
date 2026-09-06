#pragma once
#include "PlayerState.h"
#include "UI/DeadNotice.h"

// todo : 부활시키는 UI 와 비석이 떨어지면서 캐릭터는 TestActor에서 사용했던 Dead.png로 
// 비석의 5픽셀 반경으로 천천히 시계방향으로 회전하고 있는다.
// Dead 상태일땐 키보드입력은 전부 입력되지않으며 마우스로 UI에서 부활 시켜줘야 복구된다.
class PlayerDeadState : public PlayerState
{
public:
	PlayerDeadState() = default;
	virtual ~PlayerDeadState() = default;

public:
	virtual Ptr<PlayerState> HandleInput(Ptr<class PlayerComponent> PlayerComponent, Ptr<InputAction> Action, INPUT_TYPE::eType ButtonEvent) override;

	virtual ePlayerAnimationType GetAnimationType() const override;
};


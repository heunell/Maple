#include "pch.h"
#include "PlayerNoneActionState.h"
#include "PlayerShootState.h"

Ptr<PlayerState> PlayerNoneActionState::HandleInput(Ptr<class PlayerComponent> PlayerComponent, Ptr<InputAction> Action, INPUT_TYPE::eType ButtonEvent)
{
    if (!PlayerComponent || !Action)
    {
        return nullptr;
    }

    if (Action->GetName() == "SongOfHeaven" && ButtonEvent == INPUT_TYPE::DOWN)
    {
        return New<PlayerShootState>();
    }

    return nullptr;
}


ePlayerAnimationType PlayerNoneActionState::GetAnimationType() const
{
    return ePlayerAnimationType::None;
}
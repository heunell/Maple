#pragma once
#include "Core/Object.h"

class PlayerBlackBoard : public Object
{
public:
    PlayerBlackBoard() = default;
    virtual ~PlayerBlackBoard() = default;
    PlayerBlackBoard(const PlayerBlackBoard&) = delete;
    PlayerBlackBoard(PlayerBlackBoard&&) = delete;
    PlayerBlackBoard& operator = (const PlayerBlackBoard&) = delete;
    PlayerBlackBoard& operator = (PlayerBlackBoard&&) = delete;
    
public:
    void Destroy() override;
};
#pragma once
#include "Skill.h"

class TriflingWind : public Skill
{
public:
    TriflingWind() = default;
    virtual ~TriflingWind() = default;

private:
    int32 _SpawnIndex = 0;

public:
    virtual void Collision(float DeltaTime) override;

    virtual void Start() override;

    virtual void Update(float DeltaTime) override;

    virtual void End() override;
};
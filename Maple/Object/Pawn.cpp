#include "pch.h"
#include "Pawn.h"
#include "Controller/Controller.h"
#include "World/Level.h"

Pawn::Pawn()
{}

Pawn::~Pawn()
{}

bool Pawn::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name)
{
    Actor::Init(Id, Position, Scale, Rotator, Name);
    
    return true;
}

void Pawn::Tick(float DeltaTime)
{
    if (_Controller)
    {
        _Controller->Tick(DeltaTime);
    }

    Actor::Tick(DeltaTime);
}

void Pawn::Collision(float DeltaTime)
{
    Actor::Collision(DeltaTime);
}

void Pawn::Render(float DeltaTime)
{
    Actor::Render(DeltaTime);
}

void Pawn::Destroy()
{
    Actor::Destroy();

    Ptr<Level> level = GetLevel();

    if (!level)
    {
        return;
    }

    _Controller->SetActive(false);
}

void Pawn::SetController(Ptr<Controller> Controll)
{
    if (_Controller)
    {
        DESTROY(_Controller);
    }

    _Controller = Controll;
}

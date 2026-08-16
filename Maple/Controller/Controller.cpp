#include "pch.h"
#include "Controller.h"

bool Controller::Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name)
{
    Actor::Init(id, pos, scale, rot, name);
    SetEnable(false);
    return true;
}

void Controller::Tick(float DeltaTime)
{
    Actor::Tick(DeltaTime);
}

void Controller::Collision(float DeltaTime)
{}

void Controller::Render(float DeltaTime)
{}

void Controller::Destroy()
{
    Actor::Destroy();
}

void Controller::SetPawn(Ptr<class Pawn> pawn)
{
    _pawn = pawn;
}

Ptr<class Pawn> Controller::GetPawn() const
{
    return Lock<Pawn>(_pawn);
}

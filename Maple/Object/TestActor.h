#pragma once
#include "Actor.h"

class TestActor : public Actor
{
public:
	TestActor();
	virtual ~TestActor();

public:
	virtual bool Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name) override;
};


#pragma once
#include "SceneComponent.h"
#include <vector>

class MeshComponent : public SceneComponent
{
public:
	MeshComponent();
	virtual ~MeshComponent();

protected:
	std::vector<Ptr<class Material>> _MaterialSlots;

public:
	virtual bool Init(int32 Id, const std::string& Name, Ptr<class Actor> Owner) override;

	virtual void Tick(float DeltaTime) override;

	virtual void Collision(float DeltaTime) override;

	virtual void Render(float DeltaTime) override;

	virtual void Destroy() override;
};


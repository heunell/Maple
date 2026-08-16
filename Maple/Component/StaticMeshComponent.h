#pragma once
#include "MeshComponent.h"

class StaticMeshComponent : public MeshComponent
{
public:
	StaticMeshComponent();
	virtual ~StaticMeshComponent();

protected:
	Weak<class Mesh> _Mesh;
	Weak<class Shader> _Shader;

public:
	void SetMesh(const std::string& Name);

	void SetShader(const std::string& Name);

	void SetMaterial(int32 SlotIndex, const std::string& Name);

	void AddTexture(int32 SlotIndex, const std::string& Name, int32 RegisterNumber, int32 ShaderBufferType = SHADER_TYPE::PIXEL, int32 TextureIndex = 0);

	void SetBaseColor(int32 SlotIndex, float Red, float Green, float Blue, float Alpha);

	void SetOpacity(int SlotIndex, float Opacity);

	virtual bool Init(int32 Id, const std::string& Name, Ptr<class Actor> Owner) override;

	virtual void Tick(float DeltaTime) override;

	virtual void Collision(float DeltaTime) override;

	virtual void Render(float DeltaTime) override;

	virtual void Destroy() override;

};


#pragma once
#include "SceneComponent.h"
#include "Animation2D.h"

class SpriteComponent : public SceneComponent
{
public:
	SpriteComponent();
	virtual ~SpriteComponent();

protected:
	Ptr<class Mesh> _Mesh;

	Ptr<class Shader> _Shader;

	Ptr<class Texture> _Texture;

	Ptr<class Animation2D> _Animation;

	Ptr<class SpriteCBuffer> _SpriteCBuffer;

	int32 _TextureIndex = 0;

	FVector4D _Tint = FVector4D(1.f, 1.f, 1.f, 1.f);

public:
    virtual bool Init(int32 Id, const std::string& Name, Ptr<class Actor> Owner) override;

	virtual void Tick(float DeltaTime) override;

	virtual void Collision(float DeltaTime) override;

	virtual void Render(float DeltaTime) override;

	virtual void Destroy() override;

	void SetShader(const std::string& Name);

	void SetTint(const FVector4D& Tint);

	void SetTint(float Red, float Green, float Blue);

	void SetOpacity(float Opacity);

	void SetTexture(const std::string& Name, int TextureIndex = 0);

	void SetTexture(Ptr<class Texture> Texture, int TextureIndex = 0);

	void SetTextureIndex(int32 Index);

	void AddAnimationSequence(const std::string& Name, bool Loop = false, bool Reverse = false, float PlayTime = 1.f, float PlayRate = 1.f);

	void AddAnimationSequence(Ptr<class Animation2DData> Data, bool Loop = false, bool Reverse = false, float PlayTime = 1.f, float PlayRate = 1.f);

	void SetPlayTime(const std::string& Name, float Time);

	void SetPlayRate(const std::string& Name, float Rate);

	void SetLoop(const std::string& Name, bool Loop);

	void SetReverse(const std::string& Name, bool Reverse);

	void ChangeAnimation(const std::string& Name);

	void SetPlay(const std::string& Name, bool Play);

	void SetAnimationFlip(bool Flip);

	Ptr<class Animation2D> CreateAnimation();

	Ptr<class Animation2D> GetAnimation();

	template<typename T>
	void AddNotify(const std::string& SequenceName, int32 Frame, T* Object, void(T::* MemberFunction)())
	{
		_Animation->AddNotify(SequenceName, Frame, Object, MemberFunction);
	}

	template<typename T>
	void AddNotify(const std::string& SequenceName, int32 Frame, T&& Function)
	{
		_Animation->AddNotify(SequenceName, Frame, std::forward<T>(Function));
	}
};
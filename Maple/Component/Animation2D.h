#pragma once
#include "Core/Object.h"
#include "Core/Animation2DSequence.h"
#include <unordered_map>

class Animation2D : public Object
{
	friend class SpriteComponent;

public:
	Animation2D();
	virtual ~Animation2D();

private:
	Ptr<class AnimationCBuffer> _AnimationBuffer;

	Weak<class SpriteComponent> _Owner;

	Ptr<class Animation2DSequence> _CurrentSequence;

	std::unordered_map<std::string, Ptr<class Animation2DSequence>> _Sequence;

	bool _Flip = false;

public:
	void SetAnimationFlip(bool Flip);

	bool Init();

	void Tick(float DeltaTime);

	bool AddSequence(const std::string& Name, bool Loop = false, bool Reverse = false, float PlayTime = 1.f, float PlayRate = 1.f);

	bool AddSequence(Ptr<class Animation2DData> Data, bool Loop = false, bool Reverse = false, float PlayTime = 1.f, float PlayRate = 1.f);

	void SetPlayTime(const std::string& Name, float PlayTime);

	void SetPlayRate(const std::string& Name, float PlayRate);

	void SetLoop(const std::string& Name, bool Loop);

	void SetReverse(const std::string& Name, bool Reverse);

	void SetPlay(const std::string& Name, bool Play);

	void SetFrame(int32 Frame);

	void ChangeAnimation(const std::string& Name);

	void SetShader();

	bool IsFinished() const;

	virtual void Destroy() override;

private:
	Ptr<Animation2DSequence> FindSequence(const std::string& Name);

public:
	template<typename T>
	void AddNotify(const std::string& Name, int32 Frame, T* Object, void(T::* MemberFunction)())
	{
		Ptr<Animation2DSequence> Sequence = FindSequence(Name);

		if (nullptr == Sequence)
		{
			return;
		}

		Sequence->AddNotify(Frame, Object, MemberFunction);
	}

	template<typename T>
	void AddNotify(const std::string& Name, int32 Frame, T&& Function)
	{
		Ptr<Animation2DSequence> Sequence = FindSequence(Name);

		if (nullptr == Sequence)
		{
			return;
		}

		Sequence->AddNotify(Frame, std::forward<T>(Function));
	}
};


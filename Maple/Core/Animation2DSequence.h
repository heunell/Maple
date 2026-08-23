#pragma once
#include "Object.h"
#include "Animation2DData.h"
#include <functional>
#include <map>

struct FAnimation2DNotify
{
	int32 _FrameNotify;

	std::vector<std::function<void()>> _Functions;
};

class Animation2DSequence : public Object
{
	friend class Animation2D;

public:
	Animation2DSequence();

	virtual ~Animation2DSequence();

private:
	Weak<class Animation2D> _Owner;

	Ptr<class Animation2DData> _Data;

	int32 _Frame = 0;

	float _Time = 0;

	float _FrameTime = 0;

	float _PlayTime = 0;

	float _PlayRate = 0;

	bool _Loop = false;

	bool _Reverse = false;

	bool _Play = true;

	std::map<int32, FAnimation2DNotify> _Notify;

public:
	Ptr<class Animation2DData> GetAnimation2DData() const;

	int32 GetFrame() const;

	const float GetPlayTime() const;

	const std::string& GetName() const;

	void SetData(Ptr<class Animation2DData> Data);

	void SetPlayTime(float PlayTime);

	void SetPlayRate(float RateTime);

	void SetLoop(bool Loop);

	void SetReverse(bool Reverse);

	void SetPlay(bool Play);

	void Tick(float DeltaTime);

	bool IsFinished() const;

	virtual void Destroy() override;

private:
	void InvokeNotify();

	template<typename T>
	void AddNotify(int32 Frame, T* Object, void(T::* MemberFunction)())
	{
		// todo : Frame 처리
		int32 Count = _Data->GetFrameCount();

		if (Frame >= Count || Frame < 0)
		{
			return;
		}

		auto It = _Notify.find(Frame);

		if (_Notify.end() != It)
		{
			It->second._Functions.push_back(std::bind(MemberFunction, Object));

			return;
		}

		FAnimation2DNotify Notify;

		Notify._FrameNotify = Frame;

		Notify._Functions.push_back(std::bind(MemberFunction, Object));

		_Notify[Frame] = Notify;
	}

	template<typename T>
	void AddNotify(int32 Frame, T&& Function)
	{
		auto It = _Notify.find(Frame);
		if (_Notify.end() != It)
		{
			It->second._Functions.push_back(std::forward<T>(Function));

			return;
		}

		FAnimation2DNotify Notify;

		Notify._FrameNotify = Frame;

		Notify._Functions.push_back(std::forward<T>(Function));

		_Notify[Frame] = Notify;
	}

};


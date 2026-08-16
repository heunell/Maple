#pragma once
#include "Common/Singleton.h"
#include "Timer.h"
#include <chrono>
#include <map>

class TimeManager : public Singleton<TimeManager>
{
public:
	TimeManager() = default;
	virtual ~TimeManager() = default;
	TimeManager(const TimeManager&) = delete;
	TimeManager(TimeManager&&) = delete;
	TimeManager& operator = (const TimeManager&) = delete;
	TimeManager& operator = (TimeManager&&) = delete;


private:
	std::chrono::high_resolution_clock::time_point _PreviewTime;

	float _DeltaTime = 0.f;

	std::map<int32, Ptr<class Timer>> _Timers;

	std::vector<int32> _RemoveTimers;

	int32 _TimerID = 0;

public:
	bool Init();

	virtual void Destroy() override;

	float Tick();

	float GetDeltaTime() const;

	float GetFPS();

	void RemoveTimer(int32 Id);

	template<typename T>
	int32 SetTimer(float ExpirationTime, bool Repeat, T&& Function)
	{
		int32 Id = _TimerID++;

		Ptr<Timer> Timers = New<Timer>();

		Timers->SetTimer(ExpirationTime, Repeat, std::forward<T>(Function));

		_Timers[Id] = Timers;

		return Id;
	}

	template<typename T>
	int32 SetTimer(float ExpirationTime, bool Repeat, T* Object, void(T::* Function)())
	{
		int32 Id = _TimerID++;

		Ptr<Timer> Timers = New<Timer>();

		Timers->SetTimer(ExpirationTime, Repeat, Object, Function);

		_Timers[Id] = Timers;

		return Id;
	}
};


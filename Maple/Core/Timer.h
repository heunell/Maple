#pragma once
#include "Object.h"
#include <functional>

class Timer : Object
{
public:
	Timer() = default;
    virtual ~Timer() = default;
    Timer(const Timer&) = delete;
    Timer(Timer&&) = delete;
    Timer& operator=(const Timer&) = delete;
    Timer& operator=(Timer&&) = delete;

public:
    float _ExpirationTime  = 0.f; // 만료 시간

    float _AccumulatedTime = 0.f; // 누적 시간

    bool  _Run = false;

    bool  _Repeat = false;

    std::function<void()> _CallBack; // 타이머가 지정된 시간에 따라 해야할 일을 바인딩

    const bool IsRun() const;

    void Tick(float DeltaTime);

    void Stop();

    virtual void Destroy() override;

    template<typename T>
    void SetTimer(float ExpirationTime, bool Repeat, T&& Function)
    {
        if (_Run)
        {
            return;
        }

        _ExpirationTime = ExpirationTime;

        _Repeat = Repeat;

        _CallBack = std::forward<T>(Function);

        _AccumulatedTime = 0.f;

        _Run = true;
    }

    template<typename T>
    void SetTimer(float ExpirationTime, bool Repeat, T* Object, void(T::* Function)())
    {
        if (_Run)
        {
            return;
        }

        _ExpirationTime = ExpirationTime;

        _Repeat = Repeat;

        _CallBack = std::bind(Function, Object);

        _AccumulatedTime = 0.f;

        _Run = true;
    }

};


#include "pch.h"
#include "Timer.h"

const bool Timer::IsRun() const
{
    return false;
}

void Timer::Tick(float DeltaTime)
{
    if (!_Run)
    {
        return;
    }

    _AccumulatedTime += DeltaTime;

    if (_AccumulatedTime < _ExpirationTime)
    {
        return;
    }

    _AccumulatedTime -= _ExpirationTime;
    
    if (_CallBack)
    {
        _CallBack();
    }

    if (!_Repeat)
    {
        _Run = false;
    }
}

void Timer::Stop()
{
    _Run = false;

    _AccumulatedTime = 0.f;
}

void Timer::Destroy()
{
}

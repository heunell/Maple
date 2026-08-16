#include "pch.h"
#include "TimeManager.h"

bool TimeManager::Init()
{
    _PreviewTime = std::chrono::high_resolution_clock::now();

    return true;
}

void TimeManager::Destroy()
{
    for (auto& It : _Timers)
    {
        DESTROY(It.second);
    }

    _Timers.clear();

    _RemoveTimers.clear();
}

float TimeManager::Tick()
{
    auto CurrentTime = std::chrono::high_resolution_clock::now();

    std::chrono::duration<float> FrameTime = CurrentTime - _PreviewTime;

    _DeltaTime = FrameTime.count();

    _PreviewTime = CurrentTime;

    for (int32 Id : _RemoveTimers)
    {
        auto It = _Timers.find(Id);

        if (_Timers.end() == It)
        {
            continue;
        }

        DESTROY(It->second);

        _Timers.erase(Id);
    }

    _RemoveTimers.clear();

    for (auto& It : _Timers)
    {
        if (nullptr == It.second)
        {
            RemoveTimer(It.first);
            continue;
        }

        It.second->Tick(_DeltaTime);
        if (!It.second->IsRun())
            RemoveTimer(It.first);
    }

    return _DeltaTime;
}

float TimeManager::GetDeltaTime() const
{
    return _DeltaTime;
}

float TimeManager::GetFPS()
{
    if (_DeltaTime <= 0.f)
    {
        return 0.f;
    }

    return 1.f / _DeltaTime;
}

void TimeManager::RemoveTimer(int32 Id)
{
    _RemoveTimers.push_back(Id);
}

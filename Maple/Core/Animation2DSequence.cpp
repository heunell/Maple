#include "pch.h"
#include "Animation2DSequence.h"
#include "GameEngine.h"

Animation2DSequence::Animation2DSequence()
{}

Animation2DSequence::~Animation2DSequence()
{}

Ptr<class Animation2DData> Animation2DSequence::GetAnimation2DData() const
{
    return _Data;
}

int32 Animation2DSequence::GetFrame() const
{
    return _Frame;
}

const float Animation2DSequence::GetPlayTime() const
{
    return _PlayTime;
}

const std::string& Animation2DSequence::GetName() const
{
    return _Data->GetName();
}

void Animation2DSequence::SetData(Ptr<class Animation2DData> Data)
{
    _Data = Data;
}

void Animation2DSequence::SetPlayTime(float PlayTime)
{
    _PlayTime = PlayTime;

    if (_Data)
    {
        _FrameTime = PlayTime / _Data->GetFrameCount();
    }
}

void Animation2DSequence::SetPlayRate(float RateTime)
{
    _PlayRate = RateTime;
}

void Animation2DSequence::SetLoop(bool Loop)
{
    _Loop = Loop;
}

void Animation2DSequence::SetReverse(bool Reverse)
{
    _Reverse = Reverse;
}

void Animation2DSequence::SetPlay(bool Play)
{
    _Play = Play;
}

void Animation2DSequence::Tick(float DeltaTime)
{
    if (!_Play)
    {
        return;
    }

    _Time += DeltaTime * _PlayRate;

    if (_Time >= _FrameTime)
    {
        _Time -= _FrameTime;

        if (_Reverse)
        {
            --_Frame;

            if (_Frame < 0)
            {
                if (_Loop)
                {
                    _Frame = _Data->GetFrameCount() - 1;
                }
                else
                {
                    _Frame = 0;

                    SetPlay(false);
                }
            }
        }
        else
        {
            ++_Frame;

            if (_Frame == _Data->GetFrameCount() - 1)
            {
                if (_Loop)
                {
                    _Frame = 0;
                }
                else
                {
                    _Frame = _Data->GetFrameCount() - 1;

                    SetPlay(false);
                }
            }
        }

        InvokeNotify();
    }
}

void Animation2DSequence::Destroy()
{
}

void Animation2DSequence::InvokeNotify()
{
    auto It = _Notify.find(_Frame);

    if (_Notify.end() == It)
    {
        return;
    }

    FAnimation2DNotify& ItValue = It->second;

    size_t Size = ItValue._Functions.size();

    for (size_t i = 0; i < Size; ++i)
    {
        if (ItValue._Functions[i])
        {
            ItValue._Functions[i]();
        }
    }
}

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

void Animation2DSequence::SetFrame(int32 Frame)
{
    if(!_Data)
    {
        return;
    }

    if(Frame < 0 || Frame >= _Data->GetFrameCount())
    {
        return;
    }

    _Frame = Frame;

    _Time = 0.f;
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

    const int32 FrameCount = _Data->GetFrameCount();

    if (FrameCount <= 1 || _PlayRate <= 0.f)
    {
        return;
    }

    _Time += DeltaTime * _PlayRate;

    while (_Play)
    {
        const FAnimationFrame& Frame = _Data->GetFrame(_Frame);

        const float FrameDelay = Frame._Delay;

        if (FrameDelay <= 0.f || _Time < FrameDelay)
        {
            break;
        }

        _Time -= FrameDelay;

        if (!_Reverse)
        {
            if (_Frame < FrameCount - 1)
            {
                ++_Frame;
            }
            else if (_Loop)
            {
                _Reverse = true;

                _Frame = FrameCount - 2;
            }
            else
            {
                SetPlay(false);
            }
        }
        else
        {
            if (_Frame > 0)
            {
                --_Frame;
            }
            else if (_Loop)
            {
                _Reverse = false;

                _Frame = 1;
            }
            else
            {
                SetPlay(false);
            }
        }

        InvokeNotify();
    }

    //if (_Time >= _FrameTime)
    //{
    //    _Time -= _FrameTime;

    //    if (_Reverse)
    //    {
    //        --_Frame;

    //        if (_Frame < 0)
    //        {
    //            if (_Loop)
    //            {
    //                _Frame = _Data->GetFrameCount() - 1;
    //            }
    //            else
    //            {
    //                _Frame = 0;

    //                SetPlay(false);
    //            }
    //        }
    //    }
    //    else
    //    {
    //        ++_Frame;

    //        if (_Frame == _Data->GetFrameCount() - 1)
    //        {
    //            if (_Loop)
    //            {
    //                _Frame = 0;
    //            }
    //            else
    //            {
    //                _Frame = _Data->GetFrameCount() - 1;

    //                SetPlay(false);
    //            }
    //        }
    //    }

    //    InvokeNotify();
    //}
}

bool Animation2DSequence::IsFinished() const
{
    return !_Loop && !_Play;
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

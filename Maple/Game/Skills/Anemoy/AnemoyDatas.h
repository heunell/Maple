#pragma once

#include "Common/Info.h"

struct FAnemoyDatas
{
    float Cooldown             = 60.f;
                               
    float LifeTime             = 20.f;
                               
    float CastToSummonTime     = 2.10f;
                               
    float SummonActionTime     = 0.90f;

    float TraceOffsetX         = -95.f;
                               
    float TraceOffsetY         = -75.f;
                               
    float TraceSpeed           = 420.f;
                               
    float TraceSnapDistance    = 900.f;
                               
    float TraceEpsilon         = 1.f;

    float AttackAllowDistance  = 130.f;

    float AttackRangeWidth     = 760.f;
                               
    float AttackRangeHeight    = 460.f;
                               
    float AttackRangeOffsetY   = -40.f;

    float AttackCooldown       = 1.10f;
                               
    float AttackHitTime        = 0.45f;
                               
    float AttackActionTime     = 1.20f;

    float CastOffsetX          = 80.f;
                               
    float CastOffsetY          = -100.f;
                               
    float SummonOffsetX        = -17.f;
                               
    float SummonOffsetY        = -43.f;
                               
    float AttackOffsetX        = 20.f;
                               
    float AttackOffsetY        = -45.f;
                               
    int32 AttackDamageLines    = 3;

    float AttackDamageMin      = 2200000.f;
                               
    float AttackDamageMax      = 3600000.f;
};
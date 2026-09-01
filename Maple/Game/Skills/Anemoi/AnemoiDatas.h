#pragma once

#include "Common/Info.h"

struct FAnemoiDatas
{
    float Cooldown               = 60.f;   // 스킬 쿨타임
                                 
    float LifeTime               = 20.f;   // 유지시간

    FVector3D TraceRightPosition = FVector3D(-95.f, -75.f, 0.f); // 플레이어가 오른쪽을 볼 때 추적 위치

    FVector3D TraceLeftPosition  = FVector3D(95.f, -75.f, 0.f);  // 플레이어가 왼쪽을 볼 때 추적 위치
                               
    float TraceSpeed             = 420.f;  // 추적 속도
                                 
    float TraceSnapDistance      = 900.f;  // 추적 위치와 현재 위치가 멀어지면 위치보정할 거리
                                 
    float TraceStopDistance      = 1.f;    // 추적 위치와 현재 위치가 이 거리 이하라면 멈추기
                                
    float AttackStartDistance    = 130.f;  // 공격범위
                                
    float AttackRangeWidth       = 760.f;  // 공격 탐색 범위
                                 
    float AttackRangeHeight      = 460.f;  
                                 
    float AttackRangeOffsetY     = -40.f;
                                
    float AttackInterval         = 1.10f;  // 공격 인터벌
                                 
    int32 AttackDamageLines      = 3;      // 데미지 몇줄씩 뜨게 할건지
                                
    float AttackDamageMin        = 2200000.f;  // 데미지 범위
                                 
    float AttackDamageMax        = 3600000.f;
};
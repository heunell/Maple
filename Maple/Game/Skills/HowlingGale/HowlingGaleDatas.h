#pragma once

#include "Common/Info.h"

struct FHowlingGaleDatas
{
    float Cooldown = 20.f;           // 스킬 쿨타임

    float ChargeTime = 0.5f;         // 2단계가 되는 데 필요한 홀드 시간

    float LifeTime = 20.f;           // 돌풍 유지시간

    float MoveSpeed = 200.f;         // 돌풍 이동속도

    float HitMoveSpeed = 100.f;      // 몬스터 타격 중 이동속도

    FVector3D RightPosition = FVector3D(100.f, 0.f, 0.f); // 오른쪽 발사 위치

    FVector3D LeftPosition = FVector3D(-100.f, 0.f, 0.f); // 왼쪽 발사 위치

    float Stack1RangeWidth = 300.f;   // 1단계 공격 범위 너비

    float Stack1RangeHeight = 300.f;  // 1단계 공격 범위 높이

    float Stack2RangeWidth = 500.f;   // 2단계 공격 범위 너비

    float Stack2RangeHeight = 500.f;  // 2단계 공격 범위 높이

    float HitInterval = 0.25f;        // 연속 공격 간격

    int32 DamageLines = 3;            // 한 번 공격할 때 출력할 데미지 줄 수

    float Stack1DamageMin = 900000.f; // 1단계 최소 데미지

    float Stack1DamageMax = 1300000.f;// 1단계 최대 데미지

    float Stack2DamageMin = 1300000.f;// 2단계 최소 데미지

    float Stack2DamageMax = 1900000.f;// 2단계 최대 데미지
};

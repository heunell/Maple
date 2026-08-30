#include "pch.h"
#include "UI.h"
#include "Core/Device.h"

bool UI::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name)
{
    const FResolution& Resolution = Device::Instance().GetRS();

    // UI의 (0, 0)은 화면 중앙을 기준으로 사용한다.
    const FVector3D ScreenPosition(
        static_cast<float>(Resolution._Width) * 0.5f + Position._x,
        static_cast<float>(Resolution._Height) * 0.5f + Position._y,
        Position._z
    );

    return Actor::Init(Id, ScreenPosition, Scale, Rotator, Name);
}

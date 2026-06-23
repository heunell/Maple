#pragma once
#include <math.h>
#include "EngineMath.h"

union FMatrix;
struct FVector3D
{
    float _x = 0.f;
    float _y = 0.f;
    float _z = 0.f;

    FVector3D();
    FVector3D(float x, float y, float z);
    FVector3D(const FVector3D& other);
    FVector3D(FVector3D&& other) noexcept; 
    FVector3D(const DirectX::XMVECTOR v);

    const FVector3D& operator=(const FVector3D& other);
    const FVector3D& operator=(float value);
    const FVector3D& operator=(double value);
    const FVector3D& operator=(int32 value);

    FVector3D operator+(const FVector3D& other) const;
    FVector3D operator+(const float value) const;
    FVector3D operator+(const double value) const;
    FVector3D operator+(const int32 value) const;

    const FVector3D& operator+=(const FVector3D& other);
    const FVector3D& operator+=(const float value);
    const FVector3D& operator+=(const double value);
    const FVector3D& operator+=(const int32 value);

    const FVector3D& operator ++();
    FVector3D operator ++(int);

    FVector3D operator-(const FVector3D& other) const;
    FVector3D operator-(const float other) const;
    FVector3D operator-(const double other) const;
    FVector3D operator-(const int32 other) const;

    const FVector3D& operator-=(const FVector3D& other);
    const FVector3D& operator-=(const float value);
    const FVector3D& operator-=(const double value);
    const FVector3D& operator-=(const int32 value);

    const FVector3D& operator --();
    FVector3D operator --(int);

    FVector3D operator*(const FVector3D& other) const;
    FVector3D operator*(const float other) const;
    FVector3D operator*(const double other) const;
    FVector3D operator*(const int32 other) const;

    const FVector3D& operator*=(const FVector3D& other);
    const FVector3D& operator*=(const float value);
    const FVector3D& operator*=(const double value);
    const FVector3D& operator*=(const int32 value);

    FVector3D operator/(const FVector3D& other) const;
    FVector3D operator/(const float other) const;
    FVector3D operator/(const double other) const;
    FVector3D operator/(const int32 other) const;

    const FVector3D& operator/=(const FVector3D& other);
    const FVector3D& operator/=(const float value);
    const FVector3D& operator/=(const double value);
    const FVector3D& operator/=(const int32 value);

    bool operator ==(const FVector3D& other);
    bool operator !=(const FVector3D& other);

    FVector3D operator-() const;

    float Length() const;
    float Size() const;

    void Normalize();

    float Dot(const FVector3D& other) const;

    FVector3D Cross(const FVector3D& other) const;

    float Distance(const FVector3D& other) const;

    FVector3D TransformNormal(const FMatrix& mat) const; 
    FVector3D TransformCoord(const FMatrix& mat) const; 

    static FVector3D Normalize(const FVector3D& other);

    DirectX::XMVECTOR Convert() const
    {
        return DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)this);
    }
    static FVector3D Zero;
    static FVector3D Axis_X;
    static FVector3D Axis_Y;
    static FVector3D Axis_Z;
} typedef FRotator;


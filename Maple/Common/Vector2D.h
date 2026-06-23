#pragma once
#include <math.h>

struct FVector2D
{
    using F2D = FVector2D;

    float _x = 0.f;
    float _y = 0.f;

    FVector2D();
    FVector2D(float x, float y);
    FVector2D(const FVector2D& other);
    FVector2D(FVector2D&& other) noexcept ;
    
    const FVector2D& operator=(const FVector2D& other);
    const FVector2D& operator=(float value);
    const FVector2D& operator=(double value);
    const FVector2D& operator=(int32 value);
    
    FVector2D operator+(const FVector2D& other) const;
    FVector2D operator+(const float value) const;
    FVector2D operator+(const double value) const;
    FVector2D operator+(const int32 value) const;
    
    const FVector2D& operator+=(const FVector2D& other);
    const FVector2D& operator+=(const float value);
    const FVector2D& operator+=(const double value);
    const FVector2D& operator+=(const int32 value);
    
    const FVector2D& operator ++();
    FVector2D operator ++(int);

    FVector2D operator-(const FVector2D& other) const;
    FVector2D operator-(const float other) const;
    FVector2D operator-(const double other) const;
    FVector2D operator-(const int32 other) const;

    const FVector2D& operator-=(const FVector2D& other);
    const FVector2D& operator-=(const float value);
    const FVector2D& operator-=(const double value);
    const FVector2D& operator-=(const int32 value);

    const FVector2D& operator --();
    FVector2D operator --(int);

    FVector2D operator*(const FVector2D& other) const;
    FVector2D operator*(const float other) const;
    FVector2D operator*(const double other) const;
    FVector2D operator*(const int32 other) const;

    const FVector2D& operator*=(const FVector2D& other);
    const FVector2D& operator*=(const float value);
    const FVector2D& operator*=(const double value);
    const FVector2D& operator*=(const int32 value);

    FVector2D operator/(const FVector2D& other) const;
    FVector2D operator/(const float other) const;
    FVector2D operator/(const double other) const;
    FVector2D operator/(const int32 other) const;

    const FVector2D& operator/=(const FVector2D& other);
    const FVector2D& operator/=(const float value);
    const FVector2D& operator/=(const double value);
    const FVector2D& operator/=(const int32 value);

    bool operator ==(const FVector2D& other);
    bool operator !=(const FVector2D& other);

    float Length() const;
    float Size() const;

    void Normalize();

    float Dot(const FVector2D& other) const;

    float Distance(const FVector2D& other) const;

    static FVector2D Normalize(const FVector2D& other);

    static FVector2D Zero;
};


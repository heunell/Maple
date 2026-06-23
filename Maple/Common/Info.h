#pragma once

class Utility
{
public:
    template<typename T>
    static T Clamp(const T& value, const T& minValue, const T& maxValue)
    {
        return max(minValue, min(value, maxValue));
    }
};

struct FResolution
{
    uint32 _width = 0;
    uint32 _height = 0;
};

struct FVertexBuffer
{
    ComPtr<ID3D11Buffer> _buffer;
    int32 _size = 0;
    int32 _count = 0;
    std::vector<byte> _data;
};

struct FIndexBuffer
{
    ComPtr<ID3D11Buffer> _buffer = nullptr;
    int32 _size = 0;
    int32 _count = 0;
    DXGI_FORMAT _fmt = DXGI_FORMAT_UNKNOWN;
    std::vector<byte> _data;
};

struct FVertexColor
{
    FVertexColor() {}
    FVertexColor(const FVector3D& pos, const FVector4D& color) : _pos(pos), _color(color) {}
    FVertexColor(float x, float y, float z, float r, float g, float b, float a)
    {
        _pos._x = x;
        _pos._y = y;
        _pos._z = z;
        
        _color._x = r;
        _color._y = g;
        _color._z = b;
        _color._w = a;
    }
    
    FVector3D _pos;
    FVector4D _color;
};

struct FTransform
{
    FVector3D _scale;
    FVector3D _position;
    FRotator _rotation;
};

struct FTransformMatrix
{
    FMatrix _scale;
    FMatrix _rotation;
    FMatrix _translate;
    FMatrix _world;
};

struct FAABB2D
{
    FVector2D _min;
    FVector2D _max;
};

struct FOBB2D
{
    FVector2D _center;
    FVector2D _axis[2];
    FVector2D _halfSize;
};

struct FVertexTexture
{
    FVector3D _pos;
    FVector2D _uv;
    
    FVertexTexture(float x, float y, float z, float u, float v) : _pos(x,y,z), _uv(u,v) {}
};

struct FAnimationFrame
{
    FVector2D _start;
    FVector2D _size;
};

enum class eObjectType
{
    OBJECT_TYPE_TEST_OBJ,
    END
};

enum class eAssetType
{
    MESH,
    TEXTURE,
    MATERIAL,
    ANIMATION,
    SOUND,
    FONT,
    END
};

enum class eActorType
{
    Actor,
    Pawn,
    Player,
    Monster,
    End
};

namespace COMPONENT_TYPE
{
    enum Type
    {
        SCENE,
        STATICMESH,
        MESH,
        COLLISION,
        AABB,
        OBB,
        SPHERE,
        SPRITE,
        CAMERA,
        SOUND,
        TILE,
        WIDGET,
        
        //actor
        ACTOR = 500,
        MOVEMENT,
        AI,
        INPUT,
        END
    };
}
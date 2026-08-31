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
    uint32 _Width = 0;
    uint32 _Height = 0;
};

struct FVertexBuffer
{
    ComPtr<ID3D11Buffer> _Buffer = nullptr;
    int32 _Size = 0;
    int32 _Count = 0;
    std::vector<byte> _Data;
};

struct FIndexBuffer
{
    ComPtr<ID3D11Buffer> _buffer = nullptr;
    int32 _Size = 0;
    int32 _Count = 0;
    DXGI_FORMAT _Format = DXGI_FORMAT_UNKNOWN;
    std::vector<byte> _Data;
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
    FMatrix _World;

};

struct FAABB2D
{
    FVector2D _Min;
    FVector2D _Max;
};

struct FOBB2D
{
    FVector2D _center;
    FVector2D _Axis[2];
    FVector2D _halfSize;
};

struct FVertexTexture
{
    FVector3D _pos;
    FVector2D _uv;

    FVertexTexture(float x, float y, float z, float u, float v) : _pos(x, y, z), _uv(u, v) {}
};

struct FAnimationFrame
{
    FVector2D _UVLeftTop;
    
    FVector2D _UVRightBottom;

    FVector2D _Size;
    
    FVector2D _Offset;

    float _Delay = 0.f;
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

namespace SHADER_TYPE
{
    enum eType
    {
        VERTEX = 0x1,
        PIXEL = 0x2,
        GRAPHIC = VERTEX | PIXEL
    };
}

namespace INPUT_TYPE
{
    enum eType
    {
        DOWN,       
        HOLD,      
        UP,         
        END
    };
}

enum class eInputSystemType
{
    DINPUT,
    WINDOW,
    END
};

namespace MOUSE_BUTTON_TYPE
{
    enum Type
    {
        LButton,
        RButton,
        Wheel,
        End
    };
}

namespace AXIS_TYPE
{
    enum Type
    {
        X,
        Y,
        Z, 
        END
    };
}

enum eCollisionChannel : unsigned char
{
    COLLISION_CHANNEL_PLAYER,
    COLLISION_CHANNEL_PLAYERATTACK,
    COLLISION_CHANNEL_MONSTER,
    COLLISION_CHANNEL_BULLET,
    COLLISION_CHANNEL_ITEM,
    COLLISION_CHANNEL_BOSS,
    COLLISION_CHANNEL_ENVIRONMENT,
    COLLISION_CHANNEL_CAMERAZONE,
    COLLISION_CHANNEL_PORTAL,
    COLLISION_CHANNEL_END,
};

enum class eCollisionShape
{
    AABB,              
    OBB,              
    Sphere,           
    End
};

enum eCollisionResponse : unsigned char
{
    COLLISION_RESPONSE_IGNORE,      
    COLLISION_RESPONSE_BLOCK,       
    COLLISION_RESPONSE_OVERLAP,     
    COLLISION_RESPONSE_END,
};

enum eCollisionState
{
    COLLISION_STATE_RELEASE,
    COLLISION_STATE_BLOCK,  
    COLLISION_STATE_OVERLAP,
    COLLISION_STATE_END
};

enum eTextureSampleType
{
    TEXTURE_SAMPLE_POINT, 
    TEXTURE_SAMPLE_LINEAR,
    TEXTURE_SAMPLE_ANISOTROPIC, 
    TEXTURE_SAMPLE_END
};


enum class eRenderState
{
    BLEND,
    DEPTHSTENCIL,
    END
};


enum class eAnimationTextureType
{
    SPRITE,
    FRAME,
    END
};

enum class eTileType
{
    NORMAL,         //갈수 있는 곳
    BLOCK,          //못 가는 곳
    END
};

namespace AI_EVENT_STATE
{
    enum Type
    {
        EXIT,
        ENTER,
        TICK,
        END
    };
}

enum class eTransitionRule
{
    AND,
    OR,
    END
};

namespace UI_BUTTON_STATE
{
    enum Type
    {
        ENABLE,
        DISABLE,
        HOVERED,
        PRESSED,
        CLICK,
        END
    };
}

namespace UI_BUTTON_EVENT_STATE
{
    enum Type
    {
        HOVERED,
        UNHOVERED,
        PRESSED,
        CLICK,
        END
    };
}

enum class BUTTONTYPE
{
    NUM,
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

enum eLevelType : unsigned char
{
    MAIN, // 서버 선택
    CHOICE, // 캐릭터 선택
    VILLAGE, // 마을
    FIELD, // 필드
    ENTRY, // 보스 입장
    BOSS_PHASE1, // 보스 1페이즈
    BOSS_PHASE2, // 보스 2페이즈
    BOSS_REWARD, // 보스 보상
    LEVEL_END
};

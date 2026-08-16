#pragma once
#include "Asset.h"
#include "Texture.h"

#include <vector>

struct FMaterialTextureInfo
{
    std::string _Name;
    Weak<class Texture> _Texture;
    int32 _RegisterNumber;
    int32 _ShaderBufferType;
    int32 _TextureIndex;
};

class Material : public Asset
{
    friend class MaterialManager;

public:
    Material();
    virtual ~Material();

protected:
    std::vector<FMaterialTextureInfo> _Textures;

    Ptr<class Shader> _PixelShader;

    Ptr<class MaterialCBuffer> _Buffer;

    eTextureSampleType _SampleType = eTextureSampleType::TEXTURE_SAMPLE_LINEAR;

    FVector4D _BaseColor = FVector4D(1.f, 1.f, 1.f, 1.f);

    float _Opacity = 1.f;

public:
    void SetBaseColor(float R, float G, float B, float A);

    void SetBaseColor(const FVector4D& Color);

    void SetOpacity(float Op);

    void SetSamplerType(eTextureSampleType Type);

    void AddTexture(Ptr<class Texture> Texture, int32 RegisterNumber, int32 ShaderBufferType = SHADER_TYPE::PIXEL, int32 TextureIndex = 0);

    void AddTexture(const std::string& Name, int32 RegisterNumber, int32 ShaderBufferType = SHADER_TYPE::PIXEL, int32 TextureIndex = 0);

    void SetPixelShader(const std::string& Name);

    void SetMaterial();

    void ResetMaterial();

    Ptr<Material> Clone();

    virtual void Destroy() override;
};

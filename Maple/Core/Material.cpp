#include "pch.h"
#include "Material.h"
#include "AssetManager.h"
#include "Shader/ShaderManager.h"
#include "Shader/MaterialCBuffer.h"
#include "Shader/Shader.h"

Material::Material()
{}

Material::~Material()
{}

void Material::SetBaseColor(float R, float G, float B, float A)
{
	_BaseColor._x = R;
	_BaseColor._y = G;
	_BaseColor._z = B;
	_BaseColor._w = A;

}

void Material::SetBaseColor(const FVector4D& Color)
{
	_BaseColor = Color;
}

void Material::SetOpacity(float Op)
{
	_Opacity = Op;
}

void Material::SetSamplerType(eTextureSampleType Type)
{
	_SampleType = Type;
}

void Material::AddTexture(Ptr<class Texture> Texture, int32 RegisterNumber, int32 ShaderBufferType, int32 TextureIndex)
{
	FMaterialTextureInfo Info;
	Info._Texture = Texture;
	Info._RegisterNumber = RegisterNumber;
	Info._ShaderBufferType = ShaderBufferType;
	Info._TextureIndex = TextureIndex;

	_Buffer->SetTextureWidth(Texture->GetTexture(TextureIndex)->_Width);
	
	_Buffer->SetTextureHeight(Texture->GetTexture(TextureIndex)->_Height);

	_Textures.emplace_back(Info);
}

void Material::AddTexture(const std::string& Name, int32 RegisterNumber, int32 ShaderBufferType, int32 TextureIndex)
{
	Ptr<Texture> Texture = TEXTURE_MANAGER->FindTexture(Name);

	if (nullptr == Texture)
	{
		return;
	}

	FMaterialTextureInfo Information;
	
	Information._Texture = Texture;
	
	Information._RegisterNumber = RegisterNumber;
	
	Information._ShaderBufferType = ShaderBufferType;
	
	Information._TextureIndex = TextureIndex;
	
	Information._Name = Texture->GetName();

	_Buffer->SetTextureWidth(Texture->GetTexture(TextureIndex)->_Width);

	_Buffer->SetTextureHeight(Texture->GetTexture(TextureIndex)->_Height);

	_Textures.emplace_back(Information);
}

void Material::SetPixelShader(const std::string& Name)
{
	_PixelShader = ShaderManager::Instance().FindShader(Name);
}

void Material::SetMaterial()
{
	_Buffer->SetBaseColor(_BaseColor);

	_Buffer->SetOpacity(_Opacity);

	_Buffer->Update();

	ShaderManager::Instance().SetSample(_SampleType);

	_PixelShader->SetShader();

	size_t TextCount = _Textures.size();

	for (size_t i = 0; i < TextCount; ++i)
	{
		Ptr<Texture> Textures = Lock<Texture>(_Textures[i]._Texture);

		if (nullptr == Textures)
		{
			continue;
		}

		Textures->SetShader(_Textures[i]._RegisterNumber, _Textures[i]._ShaderBufferType, _Textures[i]._TextureIndex);
	}
}

void Material::ResetMaterial()
{
	size_t TextureCount = _Textures.size();

	for (size_t i = 0; i < TextureCount; ++i)
	{
		Ptr<Texture> Textures = Lock<Texture>(_Textures[i]._Texture);

		if (nullptr == Textures)
		{
			continue;
		}

		Textures->ResetShader(_Textures[i]._RegisterNumber, _Textures[i]._ShaderBufferType);
	}
}

Ptr<Material> Material::Clone()
{
	Ptr<Material> Clone = New<Material>();
	
	Clone->_BaseColor = _BaseColor;
	
	Clone->_Opacity = _Opacity;
	
	Clone->_SampleType = _SampleType;
	
	Clone->_PixelShader = _PixelShader;
	
	Clone->_Buffer = _Buffer;
	
	Clone->_Textures = _Textures;
	
	return Clone;
}

void Material::Destroy()
{
}

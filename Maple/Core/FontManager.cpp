#include "pch.h"
#include "FontManager.h"
#include "Font.h"
#include "Device.h"

bool FontManager::Init()
{
	if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory5), reinterpret_cast<IUnknown**>(_WriteFactory.GetAddressOf()))))
	{
		return false;
	}

	if (!LoadFont("MalgunGothic", TEXT("맑은 고딕"), 700, 18.f, TEXT("ko-KR"), DWRITE_FONT_STRETCH_NORMAL))
	{
		return false;
	}

	if (!CreateFontColor(255, 255, 255, 255))
	{
		return false;
	}

	if (!CreateFontColor(0, 0, 0, 255))
	{
		return false;
	}

	return true;
}

void FontManager::Destroy()
{
	for (auto& FontPair : _Fonts)
	{
		DESTROY(FontPair.second);
	}

	_Fonts.clear();

	_FontColors.clear();

	_WriteFactory.Reset();
}

bool FontManager::LoadFont(const std::string& Name, const std::wstring& FontName, int32 Weight, float FontSize, const std::wstring& LocaleName, int32 Stretch)
{
	if (FindFont(Name))
	{
		return true;
	}

	Ptr<Font> FontAsset = New<Font>();

	if (!FontAsset->LoadFont(_WriteFactory.Get(), nullptr, FontName, Weight, FontSize, LocaleName,Stretch))
	{
		return false;
	}

	FontAsset->SetName(Name);

	_Fonts[Name] = FontAsset;

	return true;
}

bool FontManager::CreateFontColor(uint8 Red, uint8 Green, uint8 Blue, uint8 Alpha)
{
	if (FindFontColor(Red, Green, Blue, Alpha))
	{
		return true;
	}

	ComPtr<ID2D1SolidColorBrush> Brush;

	if (FAILED(Device::Instance().GetTarget2D()->CreateSolidColorBrush(
		D2D1::ColorF(static_cast<float>(Red)   / 255.f,
					 static_cast<float>(Green) / 255.f,
					 static_cast<float>(Blue)  / 255.f,
					 static_cast<float>(Alpha) / 255.f), Brush.GetAddressOf())))
	{
		return false;
	}

	uint32 ColorKey = Alpha;

	ColorKey = ColorKey << 8 | Red;

	ColorKey = ColorKey << 8 | Green;

	ColorKey = ColorKey << 8 | Blue;

	_FontColors[ColorKey] = Brush;

	return true;
}

Ptr<class Font> FontManager::FindFont(const std::string& Name)
{
	auto Iterator = _Fonts.find(Name);

	if (Iterator == _Fonts.end())
	{
		return nullptr;
	}

	return Iterator->second;
}

ID2D1SolidColorBrush* FontManager::FindFontColor(uint8 Red, uint8 Green, uint8 Blue, uint8 Alpha)
{
	uint32 ColorKey = Alpha;

	ColorKey = ColorKey << 8 | Red;

	ColorKey = ColorKey << 8 | Green;

	ColorKey = ColorKey << 8 | Blue;

	auto Iterator = _FontColors.find(ColorKey);

	if (Iterator == _FontColors.end())
	{
		return nullptr;
	}

	return Iterator->second.Get();
}

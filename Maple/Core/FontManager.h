#pragma once

#include "SubManager.h"

#include <map>
#include <unordered_map>

class FontManager : public SubManager
{
public:
	FontManager() = default;
	virtual ~FontManager() = default;
	FontManager(const FontManager&) = delete;
	FontManager(FontManager&&) = delete;
	FontManager& operator=(const FontManager&) = delete;
	FontManager& operator=(FontManager&&) = delete;

private:
	ComPtr<IDWriteFactory5> _WriteFactory;

	std::unordered_map<std::string, Ptr<class Font>> _Fonts;

	std::map<uint32, ComPtr<ID2D1SolidColorBrush>> _FontColors;

public:
	virtual bool Init() override;

	virtual void Destroy() override;

	bool LoadFont(const std::string& Name, const std::wstring& FontName, int32 Weight, float FontSize, const std::wstring& LocaleName, int32 Stretch);

	bool CreateFontColor(uint8 Red, uint8 Green, uint8 Blue, uint8 Alpha);

	Ptr<class Font> FindFont(const std::string& Name);

	ID2D1SolidColorBrush* FindFontColor(uint8 Red, uint8 Green, uint8 Blue, uint8 Alpha);
};
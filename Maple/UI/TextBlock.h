#pragma once

#include "Core/Widget.h"
#include "Core/Font.h"

enum class eTextAlignH
{
	LEFT,
	CENTER,
	RIGHT,
	END
};

enum class eTextAlignV
{
	TOP,
	MIDDLE,
	BOTTOM,
	END
};

class TextBlock : public Widget
{
public:
	TextBlock();
	virtual ~TextBlock() = default;
	TextBlock(const TextBlock&) = delete;
	TextBlock(TextBlock&&) = delete;
	TextBlock& operator=(const TextBlock&) = delete;
	TextBlock& operator=(TextBlock&&) = delete;

private:
	ComPtr<ID2D1RenderTarget> _RenderTarget;

	ComPtr<IDWriteTextLayout> _TextLayout;

	ID2D1SolidColorBrush* _TextColor = nullptr;

	std::wstring _Text;

	Ptr<Font> _Font;

	float _FontSize = 18.f;

	float _Opacity = 1.f;

	eTextAlignH _AlignH = eTextAlignH::CENTER;

	eTextAlignV _AlignV = eTextAlignV::MIDDLE;

private:
	void CreateTextLayout();

public:
	virtual bool Init(int32 Id, const std::string& Name, Ptr<class Actor> Owner) override;

	virtual void Render(float DeltaTime) override;

	virtual void Destroy() override;

	virtual void SetSize(const FVector2D& Size) override;

	void SetFont(const std::string& Name);

	void SetText(const std::wstring& Text);

	void SetTextColor(uint8 Red, uint8 Green, uint8 Blue, uint8 Alpha);

	void SetOpacity(float Opacity);

	void SetFontSize(float FontSize);

	void SetAlignH(eTextAlignH Align);

	void SetAlignV(eTextAlignV Align);

	const std::wstring& GetText() const;
};
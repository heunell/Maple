#include "pch.h"
#include "TextBlock.h"
#include "Core/AssetManager.h"
#include "Core/Device.h"

TextBlock::TextBlock()
{
	_IsRender = true;
}

bool TextBlock::Init(int32 Id, const std::string& Name, Ptr<class Actor> Owner)
{
	if (!Widget::Init(Id, Name, Owner))
	{
		return false;
	}

	_RenderTarget = Device::Instance().GetTarget2D();

	if (!_RenderTarget)
	{
		return false;
	}

	SetRenderLayerName("UIText");

	SetFont("MalgunGothic");

	SetTextColor(255, 255, 255, 255);

	return _Font && _TextColor;
}

void TextBlock::CreateTextLayout()
{
	_TextLayout.Reset();

	if (!_Font || _Text.empty() || _Size._x <= 0.f || _Size._y <= 0.f)
	{
		return;
	}

	IDWriteTextLayout* TextLayout = _Font->CreateLayout(_Text, static_cast<int32>(_Text.length()), _Size._x, _Size._y);

	if (!TextLayout)
	{
		return;
	}

	_TextLayout.Attach(TextLayout);

	DWRITE_TEXT_RANGE TextRange;

	TextRange.startPosition = 0;

	TextRange.length = static_cast<UINT32>(_Text.length());

	_TextLayout->SetFontSize(_FontSize, TextRange);

	switch (_AlignH)
	{
	case eTextAlignH::LEFT:
		_TextLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		break;

	case eTextAlignH::CENTER:
		_TextLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		break;

	case eTextAlignH::RIGHT:
		_TextLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		break;

	case eTextAlignH::END:
		break;
	}

	switch (_AlignV)
	{
	case eTextAlignV::TOP:
		_TextLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		break;

	case eTextAlignV::MIDDLE:
		_TextLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		break;

	case eTextAlignV::BOTTOM:
		_TextLayout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
		break;

	case eTextAlignV::END:
		break;
	}
}

void TextBlock::Render(float DeltaTime)
{
	Widget::Render(DeltaTime);

	if (!_RenderTarget || !_TextLayout || !_TextColor)
	{
		return;
	}

	const FVector3D& WorldPosition = GetWorldPosition();

	const FResolution& Resolution = Device::Instance().GetRS();

	D2D1_POINT_2F DrawPosition;

	DrawPosition.x = WorldPosition._x - _Size._x * 0.5f;

	DrawPosition.y = static_cast<float>(Resolution._Height) - WorldPosition._y - _Size._y * 0.5f;

	_TextColor->SetOpacity(_Opacity);

	_RenderTarget->BeginDraw();

	_RenderTarget->DrawTextLayout(DrawPosition, _TextLayout.Get(), _TextColor, D2D1_DRAW_TEXT_OPTIONS_CLIP);

	_RenderTarget->EndDraw();
}

void TextBlock::Destroy()
{
	_TextLayout.Reset();

	_RenderTarget.Reset();

	_TextColor = nullptr;

	_Font.reset();

	Widget::Destroy();
}

void TextBlock::SetSize(const FVector2D& Size)
{
	Widget::SetSize(Size);

	CreateTextLayout();
}

void TextBlock::SetFont(const std::string& Name)
{
	_Font = FONT_MANAGER->FindFont(Name);

	CreateTextLayout();
}

void TextBlock::SetText(const std::wstring& Text)
{
	_Text = Text;

	CreateTextLayout();
}

void TextBlock::SetTextColor(uint8 Red, uint8 Green, uint8 Blue, uint8 Alpha)
{
	_TextColor = FONT_MANAGER->FindFontColor(Red, Green, Blue, Alpha);
}

void TextBlock::SetOpacity(float Opacity)
{
	_Opacity = Opacity;
}

void TextBlock::SetFontSize(float FontSize)
{
	_FontSize = FontSize;

	CreateTextLayout();
}

void TextBlock::SetAlignH(eTextAlignH Align)
{
	_AlignH = Align;

	CreateTextLayout();
}

void TextBlock::SetAlignV(eTextAlignV Align)
{
	_AlignV = Align;

	CreateTextLayout();
}

const std::wstring& TextBlock::GetText() const
{
	return _Text;
}
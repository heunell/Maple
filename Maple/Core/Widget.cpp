#include "pch.h"
#include "Widget.h"

bool Widget::Init(int32 Id, const std::string& Name, Ptr<class Actor> Owner)
{
	if (!SceneComponent::Init(Id, Name, Owner))
	{
		return false;
	}

	return true;
}

void Widget::Destroy()
{
	SceneComponent::Destroy();
}

const FVector2D& Widget::GetSize() const
{
	return _Size;
}

void Widget::SetSize(const FVector2D& Size)
{
	_Size = Size;
}

void Widget::SetInteractable(bool Interactable)
{
	_Interactable = Interactable;
}

bool Widget::IsInteractable() const
{
	return _Interactable;
}

bool Widget::IsInside(const FVector2D& Position) const
{
	const FVector3D& WorldPosition = GetWorldPosition();

	float HalfWidth = _Size._x * 0.5f;

	float HalfHeight = _Size._y * 0.5f;

	return Position._x >= WorldPosition._x - HalfWidth  &&
		   Position._x <= WorldPosition._x + HalfWidth  &&
		   Position._y >= WorldPosition._y - HalfHeight &&
		   Position._y <= WorldPosition._y + HalfHeight;
}

void Widget::OnHovered()
{}

void Widget::OnUnHovered()
{}

void Widget::OnClick()
{}

void Widget::OnPressed()
{}
#pragma once

#include "Component/SceneComponent.h"

class Widget : public SceneComponent
{
public:
	Widget() = default;
	virtual ~Widget() = default;
	Widget(const Widget&) = delete;
	Widget(Widget&&) = delete;
	Widget& operator=(const Widget&) = delete;
	Widget& operator=(Widget&&) = delete;

protected:
	FVector2D _Size = FVector2D::Zero;

	bool _Interactable = false;

public:
	virtual bool Init(int32 Id, const std::string& Name, Ptr<class Actor> Owner) override;

	virtual void Destroy() override;

	const FVector2D& GetSize() const;

	virtual void SetSize(const FVector2D& Size);

	void SetInteractable(bool Interactable);

	bool IsInteractable() const;

	bool IsInside(const FVector2D& Position) const;

	virtual void OnHovered();

	virtual void OnUnHovered();

	virtual void OnClick();

	virtual void OnPressed();
};
#pragma once
#include "ActorComponent.h"
#include <chrono>
#include <functional>

enum class eInteractionSpace
{
	World,
	Screen,
	End
};

enum class eInteractionClickType
{
	Click,
	DoubleClick,
	Drag,

	End
};

class InteractionComponent : public ActorComponent
{
public:
	InteractionComponent() = default;
	virtual ~InteractionComponent() = default;
	InteractionComponent(const InteractionComponent&) = delete;
	InteractionComponent(InteractionComponent&&) = delete;
	InteractionComponent& operator=(const InteractionComponent&) = delete;
	InteractionComponent& operator=(InteractionComponent&&) = delete;

private:
	Weak<class SceneComponent> _InteractionTarget;

	eInteractionSpace _Space = eInteractionSpace::World;

	eInteractionClickType _ClickType = eInteractionClickType::Click;

	std::chrono::steady_clock::time_point _LastClickTime;

	std::function<void()> _InteractionCallback;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void Destroy() override;

	void SetInteractionTarget(const Ptr<class SceneComponent>& Target);

	void SetInteractionSpace(eInteractionSpace Space);

	void SetClickType(eInteractionClickType ClickType);

	 
public:
	template<typename T>
	void SetInteractionCallback(T* Object, void(T::* MemberFunction)())
	{
		_InteractionCallback = [Object, MemberFunction]()
			{
				(Object->*MemberFunction)();
			};
	}

private:
	bool IsMouseInsideTarget() const;

	void TryInteraction();


};


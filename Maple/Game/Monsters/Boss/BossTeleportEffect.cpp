#include "pch.h"
#include "BossTeleportEffect.h"
#include "BossTeleportState.h"
#include "Component/SpriteComponent.h"

bool BossTeleportEffect::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name)
{
	if (!Actor::Init(Id, Position, Scale, Rotator, Name))
	{
		return false;
	}

	_Sprite = CreateSceneComponent<SpriteComponent>("BossTeleportEffectSprite");

	if (!_Sprite)
	{
		return false;
	}

	_Sprite->SetRenderLayerName("Default");

	_Sprite->AddAnimationSequence("level.6.special", false);

	_Sprite->AttachToComponent(GetRoot());

	_Sprite->SetRelativePosition(0.f, 0.f, 0.1f);

	SetEffectEnable(false);

	return true;
}

void BossTeleportEffect::Tick(float DeltaTime)
{
	Actor::Tick(DeltaTime);

	Ptr<Actor> Target = Lock(_Target);

	if (Target)
	{
		SetWorldPosition(Target->GetWorldPosition());
	}

	Ptr<Animation2D> Animation = _Sprite->GetAnimation();

	if (!Animation || !Animation->IsFinished())
	{
		return;
	}

	Ptr<BossTeleportState> Owner = Lock(_Owner);

	if (Owner)
	{
		Owner->TeleportPlayer();
	}

	SetEffectEnable(false);
}

void BossTeleportEffect::Start(Ptr<BossTeleportState> Owner, Ptr<Actor> Target)
{
	if (!Owner || !Target)
	{
		return;
	}

	_Owner = Owner;

	_Target = Target;

	SetWorldPosition(Target->GetWorldPosition());

	SetEffectEnable(true);

	_Sprite->ChangeAnimation("level.6.special");

	_Sprite->SetAnimationFrame(0);

	_Sprite->SetPlay("level.6.special", true);
}

void BossTeleportEffect::SetEffectEnable(bool Enable)
{
	SetEnable(Enable);

	if (_Sprite)
	{
		_Sprite->SetEnable(Enable);
	}

	if (!Enable)
	{
		_Owner.reset();

		_Target.reset();
	}
}
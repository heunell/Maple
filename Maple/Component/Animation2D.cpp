#include "pch.h"
#include "Animation2D.h"
#include "SpriteComponent.h"
#include "Core/Animation2DData.h"
#include "Core/AssetManager.h"
#include "Core/Texture.h"
#include "Shader/ShaderManager.h"
#include "Shader/AnimationCBuffer.h"

Animation2D::Animation2D()
{}

Animation2D::~Animation2D()
{}

void Animation2D::SetAnimationFlip(bool Flip)
{
	_Flip = Flip;
}

bool Animation2D::Init()
{
	_AnimationBuffer = ShaderManager::Instance().FindCBuffer<AnimationCBuffer>("Animation");
	
	return true;
}

void Animation2D::Tick(float DeltaTime)
{
	_CurrentSequence->Tick(DeltaTime);
}

bool Animation2D::AddSequence(const std::string& Name, bool Loop, bool Reverse, float PlayTime, float PlayRate)
{
	Ptr<Animation2DSequence> Sequence = FindSequence(Name);

	if (Sequence)
	{
		return false;
	}

	Ptr<Animation2DData> Data = ANIMATION_MANAGER->FindAnimation(Name);

	if (!Data)
	{
		return false;
	}

	Sequence = New<Animation2DSequence>();

	Sequence->_Owner = This<Animation2D>();

	Sequence->SetData(Data);

	Sequence->SetPlayTime(PlayTime);

	Sequence->SetPlayRate(PlayRate);

	Sequence->SetLoop(Loop);

	Sequence->SetReverse(Reverse);

	Sequence->_Frame = 0;
	
	Sequence->_Time = 0;

	if (!_CurrentSequence)
	{
		_CurrentSequence = Sequence;

		if (Ptr<SpriteComponent> Component = Lock<SpriteComponent>(_Owner))
		{
			Component->SetTexture(_CurrentSequence->GetAnimation2DData()->GetTexture());
		}
	}

	_Sequence[Name] = Sequence;

	return true;
}

bool Animation2D::AddSequence(Ptr<class Animation2DData> Data, bool Loop, bool Reverse, float PlayTime, float PlayRate)
{
	Ptr<Animation2DSequence> Sequence = FindSequence(Data->GetName());

	if (Sequence)
	{
		return false;
	}

	Sequence = New<Animation2DSequence>();

	Sequence->_Owner = This<Animation2D>();

	Sequence->SetData(Data);

	Sequence->SetPlayTime(PlayTime);

	Sequence->SetPlayRate(PlayRate);

	Sequence->SetLoop(Loop);

	Sequence->SetReverse(Reverse);

	Sequence->_Frame = 0;

	Sequence->_Time = 0;

	if (!_CurrentSequence)
	{
		_CurrentSequence = Sequence;

		if (Ptr<SpriteComponent> Component = Lock<SpriteComponent>(_Owner))
		{
			Component->SetTexture(_CurrentSequence->GetAnimation2DData()->GetTexture());
		}
	}

	_Sequence[Data->GetName()] = Sequence;

	return true;
}

void Animation2D::SetPlayTime(const std::string& Name, float PlayTime)
{
	Ptr<Animation2DSequence> Sequence = FindSequence(Name);

	if (!Sequence)
	{
		return;
	}

	Sequence->SetPlayTime(PlayTime);
}

void Animation2D::SetPlayRate(const std::string& Name, float PlayRate)
{
	Ptr<Animation2DSequence> Sequence = FindSequence(Name);

	if (!Sequence)
	{
		return;
	}

	Sequence->SetPlayRate(PlayRate);
}

void Animation2D::SetLoop(const std::string& Name, bool Loop)
{
	Ptr<Animation2DSequence> Sequence = FindSequence(Name);

	if (!Sequence)
	{
		return;
	}

	Sequence->SetLoop(Loop);
}

void Animation2D::SetReverse(const std::string& Name, bool Reverse)
{
	Ptr<Animation2DSequence> Sequence = FindSequence(Name);

	if (!Sequence)
	{
		return;
	}

	Sequence->SetReverse(Reverse);
}

void Animation2D::SetPlay(const std::string& Name, bool Play)
{
	Ptr<Animation2DSequence> Sequence = FindSequence(Name);

	if (!Sequence)
	{
		return;
	}

	Sequence->SetPlay(Play);
}

void Animation2D::SetFrame(int32 Frame)
{
	if(!_CurrentSequence)
	{
		_CurrentSequence->SetFrame(Frame);
	}
}

void Animation2D::ChangeAnimation(const std::string& Name)
{
	if (!_CurrentSequence)
	{
		return;
	}

	if (_Sequence.size() < 1)
	{
		return;
	}

	if (_CurrentSequence->GetName() == Name)
	{
		return;
	}

	Ptr<Animation2DSequence> Sequence = FindSequence(Name);
	
	if (!Sequence)
	{
		return;
	}

	_CurrentSequence = Sequence;

	_CurrentSequence->_Frame = 0;

	_CurrentSequence->_Time = 0;

	_CurrentSequence->_Reverse = false;

	_CurrentSequence->SetPlay(true);

	Ptr<SpriteComponent> Component = Lock<SpriteComponent>(_Owner);

	if (Component)
	{
		Component->SetTexture(_CurrentSequence->GetAnimation2DData()->GetTexture());
	}
}

void Animation2D::SetShader()
{
	float LeftTopX = 0.f;

	float LeftTopY = 0.f;

	float RightBottomX = 1.f;

	float RightBottomY = 1.f;

	eAnimationTextureType Type = _CurrentSequence->_Data->GetAnimationTextureType();

	Ptr<Texture> Texture = _CurrentSequence->_Data->GetTexture();

	Ptr<SpriteComponent> Component = Lock<SpriteComponent>(_Owner);

	if (nullptr == Component)
	{
		return;
	}

	switch (Type)
	{
	case eAnimationTextureType::SPRITE:
	{
		const FAnimationFrame& Frame = _CurrentSequence->_Data->GetFrame(_CurrentSequence->GetFrame());

		LeftTopX = Frame._UVLeftTop._x;

		LeftTopY = Frame._UVLeftTop._y;
		
		RightBottomX = Frame._UVRightBottom._x;
		
		RightBottomY = Frame._UVRightBottom._y;

		//Component->SetTextureIndex(0);

		float OffsetX = _Flip ? -Frame._Offset._x : Frame._Offset._x;

		Component->SetRelativeScale(Frame._Size._x, Frame._Size._y, 1.f);

		Component->SetRelativePosition(OffsetX, Frame._Offset._y, 0.f);
	}
	break;

	case eAnimationTextureType::END:
		break;

	default:
		break;
	}

	_AnimationBuffer->SetUV(LeftTopX, LeftTopY, RightBottomX, RightBottomY);

	_AnimationBuffer->SetAnimationFlip(_Flip);

	_AnimationBuffer->Update();
}

bool Animation2D::IsFinished() const
{
	if (!_CurrentSequence)
	{
		return false;
	}

	return _CurrentSequence->IsFinished();
}

void Animation2D::Destroy()
{
}

Ptr<Animation2DSequence> Animation2D::FindSequence(const std::string& Name)
{
	auto It = _Sequence.find(Name);

	if (_Sequence.end() == It)
	{
		return nullptr;
	}

	return It->second;
}

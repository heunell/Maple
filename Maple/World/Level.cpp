#include "pch.h"
#include "Level.h"
#include "CameraManager.h"
#include "TagManager.h"
#include "Game/Character/Player.h"
#include "Core/Device.h"
#include "Core/DirectoryManager.h"
#include "Component/CameraComponent.h"
#include "Collision/CollisionManager.h"
#include "Component/CollisionComponent.h"
#include "UI/PlayerUI.h"
#include "UI/CursorUI.h"

Level::Level()
{}

Level::~Level()
{}

bool Level::Init(const std::string& Path)
{
	_TagManager = New<TagManager>();
	
	_TagManager->Init();

	_CameraManager = New<CameraManager>();

	_CameraManager->Init(This<Level>());

	_CollisionManager = New<CollisionManager>();

	_CollisionManager->Init();

	if (!Path.empty())
	{
		auto CachePath = DirectoryManager::Instance().GetCachePath("Resources\\Level");

		if (!CachePath.has_value() || Path.empty())
		{
			return false;
		}

		std::filesystem::path FullPath;

		if (!DirectoryManager::Instance().GetFile(CachePath.value(), Path, OUT FullPath))
		{
			return false;
		}

		std::ifstream LoadFile(FullPath, std::ios::binary);

		if (LoadFile.fail())
		{
			return false;
		}

		Load(LoadFile);

		return true;
	}

	Ptr<Player> PlayerActor = SpawnActor<Player>("Player", FVector3D(0.f, 0.f, 1.f), FVector3D(1.f, 1.f, 1.f), FRotator(0.f, 0.f, 0.f));
	
	if (!PlayerActor)
	{
		return false;
	}

	Ptr<PlayerUI> PlayerStatusUI = SpawnActor<PlayerUI>("PlayerUI", FVector3D::Zero, FVector3D(1.f, 1.f, 1.f), FRotator(0.f, 0.f, 0.f));

	if (!PlayerStatusUI)
	{
		return false;
	}

	Ptr<CursorUI> Cursor = SpawnActor<CursorUI>("CursorUI", FVector3D::Zero, FVector3D(1.f, 1.f, 1.f), FRotator(0.f, 0.f, 0.f));
	
	if (!Cursor)
	{
		return false;
	}

	return true;
}

void Level::Tick(float DeltaTime)
{
	for (auto It : _RemoveActors)
	{
		Ptr<Actor> actor = FindActor<Actor>(It);

		if (nullptr == actor)
		{
			continue;
		}

		DeleteTag(actor);

		_Actors.erase(It);
	}

	_RemoveActors.clear();

	for (auto& It : _Actors)
	{
		if (!It.second->IsActive())
		{
			RemoveActor(It.first);

			continue;
		}

		if (!It.second->IsEnable())
		{
			continue;
		}

		It.second->Tick(DeltaTime);
	}
}

void Level::Collision(float DeltaTime)
{
	_CollisionManager->Collision(DeltaTime);

	for (auto& It : _Actors)
	{
		if (!It.second->IsActive())
		{
			RemoveActor(It.first);

			continue;
		}

		if (!It.second->IsEnable())
		{
			continue;
		}

		It.second->Collision(DeltaTime);
	}
}

void Level::Render(float DeltaTime)
{
	for (auto& It : _Actors)
	{
		if (!It.second->IsActive())
		{
			RemoveActor(It.first);

			continue;
		}

		if (!It.second->IsEnable())
		{
			continue;
		}

		It.second->Render(DeltaTime);
	}
}

void Level::RenderUI(float DeltaTime)
{
	// todo : UI
}

void Level::Destroy()
{
	for (auto& It : _Actors)
	{
		DESTROY(It.second)
	}

	_Actors.clear();

	DESTROY(_TagManager);
	DESTROY(_CameraManager);
	DESTROY(_CollisionManager);
}

void Level::Save(std::ofstream& File)
{
	for (auto& It : _Actors)
	{
		if (!It.second->IsActive())
		{
			continue;
		}

		It.second->Save(File);
	}
}

void Level::Load(std::ifstream& File)
{
	for (auto& It : _Actors)
	{
		if (!It.second->IsActive())
		{
			continue;
		}

		It.second->Load(File);
	}
}

void Level::AddTag(const std::string& Tag, int32 Id)
{
	_TagManager->Add(Tag, Id);
}

void Level::DeleteTag(Ptr<class Actor> Actor)
{
	if (Actor->_tags.size() < 1)
	{
		return;
	}

	for (auto& It : Actor->_tags)
		_TagManager->Erase(It, Actor->_Id);
}

void Level::SetMainCamera(Ptr<class CameraComponent> Camera)
{
	_CameraManager->SetMainCamera(Camera);
}

Ptr<class CameraComponent> Level::GetMainCamera() const
{
	return _CameraManager->GetMainCamera();
}

const FMatrix& Level::GetViewMatrix() const
{
	return _CameraManager->GetViewMatrix();
}

const FMatrix& Level::GetProjectionMatrix() const
{
	return _CameraManager->GetProjectionMatrix();
}

const FVector3D& Level::GetCameraWorldPosition() const
{
	return _CameraManager->GetCameraWorldPosition();
}

const FMatrix& Level::GetUIProjMatrix() const
{
	return _CameraManager->GetUIProjectionMatrix();
}

void Level::AddCollision(std::pair<int32, int32>& CollisionID, Ptr<class CollisionComponent> Component)
{
	_CollisionManager->Insert(CollisionID, Component);
}

void Level::RemoveCollision(std::pair<int32, int32>& CollisionID)
{
	_CollisionManager->Remove(CollisionID);
}

Ptr<class CollisionComponent> Level::FindCollider(std::pair<int32, int32>& CollisionID)
{
	return _CollisionManager->FindCollider(CollisionID);
}

Ptr<Actor> Level::FindActor(int32 Id)
{
	auto It = _Actors.find(Id);

	if (_Actors.end() == It)
	{
		return nullptr;
	}

	return It->second;
}

void Level::FindActors(const std::string& Tag, std::vector<Ptr<class Actor>>& OutArr)
{
	std::vector<int32> ActorIDs;
	_TagManager->GetActorIDs(Tag, ActorIDs);

	if (ActorIDs.size() < 1)
	{
		return;
	}

	for (auto It : ActorIDs)
	{
		Ptr<Actor> Actor = FindActor(It);

		if (nullptr == Actor)
		{
			continue;
		}

		OutArr.push_back(Actor);
	}
}

void Level::RemoveActor(int32 Id)
{
	_RemoveActors.push_back(Id);
}

const std::map<int32, Ptr<class Actor>>& Level::GetActors() const
{
	return _Actors;
}

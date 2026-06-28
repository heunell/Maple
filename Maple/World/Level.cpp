#include "pch.h"
#include "Level.h"

Level::Level()
{}

Level::~Level()
{}

bool Level::Init(const std::string& path)
{
}

void Level::Tick(float deltaTime)
{
}

void Level::Collision(float deltaTime)
{
}

void Level::Render(float deltaTime)
{
}

void Level::RenderUI(float deltaTime)
{
}

void Level::Destroy()
{
}

void Level::Save(std::ofstream& file)
{
}

void Level::Load(std::ifstream& file)
{
}

void Level::AddTag(const std::string& tag, int32 id)
{
}

void Level::DeleteTag(Ptr<class Actor> actor)
{
}

void Level::SetMainCamera(Ptr<class CameraComponent> camera)
{
}

Ptr<class CameraComponent> Level::GetMainCamera() const
{
}

const FMatrix& Level::GetViewMatirx() const
{
}

const FMatrix& Level::GetProjMatrix() const
{
}

const FVector3D& Level::GetCameraWorldPos() const
{
}

const FMatrix& Level::GetUIProjMatrix() const
{
}

void Level::AddCollision(std::pair<int32, int32>& colID, Ptr<class CollisionComponent> comp)
{
}

void Level::RemoveCollision(std::pair<int32, int32>& colID)
{
}

Ptr<class CollisionComponent> Level::FindCollider(std::pair<int32, int32>& colID)
{
}

void Level::FindActors(const std::string& tag, std::vector<Ptr<class Actor>>& outArr)
{
}

void Level::RemoveActor(int32 id)
{
}

const std::map<int32, Ptr<class Actor>>& Level::GetActors() const
{
}

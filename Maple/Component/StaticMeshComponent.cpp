#include "pch.h"
#include "StaticMeshComponent.h"
#include "Shader/ShaderManager.h"
#include "Shader/TransformCBuffer.h"
#include "Core/Mesh.h"
#include "Core/Material.h"
#include "Core/AssetManager.h"

StaticMeshComponent::StaticMeshComponent()
{
    _IsRender = true;
}

StaticMeshComponent::~StaticMeshComponent()
{}

void StaticMeshComponent::SetMesh(const std::string& Name)
{
    Ptr<Mesh> FoundMesh = MESH_MANAGER->FindMesh(Name);

    if (nullptr == FoundMesh)
    {
        return;
    }

    _Mesh = FoundMesh;

    _MaterialSlots.clear();
    
    size_t SlotCount = FoundMesh->GetgSlotCount();

    for (size_t i = 0; i < SlotCount; ++i)
    {
        const FMeshSlot* Slot = FoundMesh->GetSlot(static_cast<int>(i));

        _MaterialSlots.emplace_back(Slot->_Material->Clone());
    }

}

void StaticMeshComponent::SetShader(const std::string& Name)
{
    _Shader = ShaderManager::Instance().FindShader(Name);
}

void StaticMeshComponent::SetMaterial(int32 SlotIndex, const std::string& Name)
{
    _MaterialSlots[SlotIndex] = MATERIAL_MANAGER->CreateMaterialInstance(Name);
}

void StaticMeshComponent::AddTexture(int32 SlotIndex, const std::string& Name, int32 RegisterNumber, int32 ShaderBufferType, int32 TextureIndex)
{
    _MaterialSlots[SlotIndex]->AddTexture(Name, RegisterNumber, ShaderBufferType, TextureIndex);
}

void StaticMeshComponent::SetBaseColor(int32 SlotIndex, float Red, float Green, float Blue, float Alpha)
{
    _MaterialSlots[SlotIndex]->SetBaseColor(Red, Green, Blue, Alpha);
}

void StaticMeshComponent::SetOpacity(int SlotIndex, float Opacity)
{
    _MaterialSlots[SlotIndex]->SetOpacity(Opacity);
}

bool StaticMeshComponent::Init(int32 Id, const std::string& Name, Ptr<class Actor> Owner)
{
    MeshComponent::Init(Id, Name, Owner);

    SetShader("StaticMeshShader");

    _Type = COMPONENT_TYPE::STATICMESH;

    return true;
}

void StaticMeshComponent::Tick(float DeltaTime)
{
    MeshComponent::Tick(DeltaTime);
}

void StaticMeshComponent::Collision(float DeltaTime)
{
    MeshComponent::Collision(DeltaTime);
}

void StaticMeshComponent::Render(float DeltaTime)
{
    MeshComponent::Render(DeltaTime);

    Ptr<Shader> Shaders = Lock<Shader>(_Shader);

    if (Shaders)
    {
        Shaders->SetShader();
    }

    Ptr<Mesh> Meshs = Lock<Mesh>(_Mesh);

    if (Meshs)
    {
        size_t SlotCount = Meshs->GetgSlotCount();

        for (size_t i = 0; i < SlotCount; ++i)
        {
            _MaterialSlots[i]->SetMaterial();

            Meshs->Render();

            _MaterialSlots[i]->ResetMaterial();
        }
    }
}

void StaticMeshComponent::Destroy()
{
    MeshComponent::Destroy();
}

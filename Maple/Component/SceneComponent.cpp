#include "pch.h"
#include "SceneComponent.h"
#include "Object/Actor.h"
#include "Shader/ShaderManager.h"
#include "Shader/TransformCBuffer.h"
#include "Render/RenderManager.h"

SceneComponent::SceneComponent() : _RenderLayerName("Default"), _IsRender(false)
{}

SceneComponent::~SceneComponent()
{}

bool SceneComponent::Init(int32 Id, const std::string& Name, Ptr<class Actor> Owner)
{
    Component::Init(Id, Name, Owner);

    SetRelativeScale(1.f,1.f,1.f);

    _TransformCBuffer = FIND_CBUFFER("Transform", TransformCBuffer);
    
    if (_IsRender)
    {
        RenderManager::Instance().AddRenderComponent(Owner->GetActorID(), This<SceneComponent>());
    }

    _Type = COMPONENT_TYPE::SCENE;

    return true;
}

void SceneComponent::Tick(float DeltaTime)
{
    Component::Tick(DeltaTime);
    for (auto& it : _Childs)
    {
        Ptr<SceneComponent> child = it.second;

        if (nullptr == child)
        {
            continue;
        }

        if (!child->IsActive() || !child->IsEnable())
        {
            continue;
        }

        child->Tick(DeltaTime);
    }
}

void SceneComponent::Collision(float DeltaTime)
{
    Component::Collision(DeltaTime);
}

void SceneComponent::Render(float DeltaTime)
{
    Component::Render(DeltaTime);
}

void SceneComponent::Destroy()
{
    for (auto& it : _Childs)
    {
        DESTROY(it.second)
    }
    _Childs.clear();

    if (_IsRender)
    {
        Ptr<Actor> owner = Lock<Actor>(_Owner);
        if (!owner)
        {
            return;
        }

        RenderManager::Instance().RemoveRenderComponent(_RenderLayerName, owner->GetActorID(), GetComponentID());
    }
    Component::Destroy();
}

void SceneComponent::Save(std::ofstream& file)
{}

void SceneComponent::Load(std::ifstream& file)
{}

void SceneComponent::SetRenderLayerName(const std::string& name)
{
    _RenderLayerName = name;
    
    RenderManager::Instance().RefreshLayer();
}

const std::string& SceneComponent::GetRenderLayerName() const
{
    return _RenderLayerName;
}

const std::map<int32, Ptr<SceneComponent>>& SceneComponent::GetChilds() const
{
    return _Childs;
}

const Ptr<SceneComponent>& SceneComponent::GetParent() const
{
    return Lock<SceneComponent>(_Parent);
}

void SceneComponent::AttachToComponent(Ptr<SceneComponent> comp)
{
    comp->AddChild(This<SceneComponent>());
}

void SceneComponent::AddChild(Ptr<SceneComponent> comp)
{
    if (nullptr == comp)
    {
        return;
    }

    if (comp->GetComponentID() == GetComponentID())
    {
        return;
    }
    
    comp->_Parent = This<SceneComponent>();
    
    _Childs[comp->_Id] = comp;
    
    UpdateTransform();
}

Ptr<SceneComponent> SceneComponent::FindComponent(int32 id) const
{
    auto it = _Childs.find(id);
    if (_Childs.end() == it)
    {
        for (auto& subIt : _Childs)
        {
            Ptr<SceneComponent> comp = subIt.second->FindComponent(id);
            
            if (nullptr != comp)
            {
                return comp;
            }
        }

        return nullptr;
    }
    else
    {
        return it->second;
    }
}

const FTransform& SceneComponent::GetWorldTransform() const
{
    return _World;
}

void SceneComponent::SetWorldTransform(const FTransform& transform)
{
    SetWorldPosition(transform._position);
    
    SetWorldScale(transform._scale);
    
    SetWorldRotation(transform._rotation);
}

void SceneComponent::SetWorldTransform(const FVector3D& pos, const FVector3D& scale, const FRotator& rot)
{
    SetWorldPosition(pos);
    
    SetWorldScale(scale);
    
    SetWorldRotation(rot);
}

const FVector3D& SceneComponent::GetWorldPosition() const
{
    return _World._position;
}

void SceneComponent::SetWorldPosition(const FVector2D& pos)
{
    SetWorldPosition(pos._x, pos._y);
}

void SceneComponent::SetWorldPosition(float x, float y)
{
    SetWorldPosition(x,y,_World._position._z);
}

void SceneComponent::SetWorldPosition(const FVector3D& pos)
{
    SetWorldPosition(pos._x, pos._y, pos._z);
}

void SceneComponent::SetWorldPosition(float x, float y, float z)
{
    if (Ptr<SceneComponent> parentComp = Lock<SceneComponent>(_Parent))
    {
        FMatrix matParentInv = parentComp->_Matrix._World;
       
        matParentInv.Inverse();
       
        _Relative._position = FVector3D(x,y,z).TransformCoord(matParentInv);
    }
    else
    {
        _Relative._position._x = x;
       
        _Relative._position._y = y;
       
        _Relative._position._z = z;
    }
    UpdateTransform();
}

const FVector3D& SceneComponent::AddWorldPosition(const FVector2D& pos)
{
    SetWorldPosition(pos._x + _World._position._x, pos._y + _World._position._y, _World._position._z);
    return _World._position;
}

const FVector3D& SceneComponent::AddWorldPosition(float x, float y)
{
    SetWorldPosition(x + _World._position._x, y+_World._position._y,_World._position._z);
    return _World._position;
}

const FVector3D& SceneComponent::AddWorldPosition(const FVector3D& pos)
{
    SetWorldPosition(pos._x + _World._position._x, pos._y + _World._position._y, pos._z + _World._position._z);
    return _World._position;
}

const FVector3D& SceneComponent::AddWorldPosition(float x, float y, float z)
{
    SetWorldPosition(x + _World._position._x, y + _World._position._y, z + _World._position._z);
    return _World._position;
}

const FVector3D& SceneComponent::AddWorldPosition(float value)
{
    SetWorldPosition(value + _World._position._x, value + _World._position._y, _World._position._z);
    return _World._position;
}

const FVector3D& SceneComponent::AddWorldPositionX(float x)
{
    SetWorldPosition(x + _World._position._x, _World._position._y, _World._position._z);
    return _World._position;
}

const FVector3D& SceneComponent::AddWorldPositionY(float y)
{
    SetWorldPosition(_World._position._x, y + _World._position._y, _World._position._z);
    return _World._position;
}

const FVector3D& SceneComponent::GetWorldScale() const
{
    return _World._scale;
}

void SceneComponent::SetWorldScale(const FVector2D& scale)
{
    SetWorldScale(scale._x, scale._y, _World._scale._z);
}

void SceneComponent::SetWorldScale(float x, float y)
{
    SetWorldScale(x, y, _World._scale._z);;
}

void SceneComponent::SetWorldScale(const FVector3D& scale)
{
    SetWorldScale(scale._x, scale._y, scale._z);
}

void SceneComponent::SetWorldScale(float x, float y, float z)
{
    if (Ptr<SceneComponent> parentComp = Lock<SceneComponent>(_Parent))
    {
        _Relative._scale._x = x / parentComp->_World._scale._x;
        _Relative._scale._y = y / parentComp->_World._scale._y;
        _Relative._scale._z = z / parentComp->_World._scale._z;
    }
    else
    {
        _Relative._scale._x = x;
        _Relative._scale._y = y;
        _Relative._scale._z = z;
    }
    UpdateTransform();
}

const FVector3D& SceneComponent::AddWorldScale(const FVector2D& scale)
{
    SetWorldScale(scale._x + _World._scale._x, scale._y + _World._scale._y, _World._scale._z);
    return _World._scale;
}

const FVector3D& SceneComponent::AddWorldScale(float x, float y)
{
    SetWorldScale(x + _World._scale._x, y + _World._scale._y, _World._scale._z);
    return _World._scale;
}

const FVector3D& SceneComponent::AddWorldScale(const FVector3D& scale)
{
    SetWorldScale(scale._x +_World._scale._x, scale._y + _World._scale._y, scale._z + _World._scale._z);
    return _World._scale;
}

const FVector3D& SceneComponent::AddWorldScale(float x, float y, float z)
{
    SetWorldScale(x + _World._scale._x, y + _World._scale._y, z + _World._scale._z);
    return _World._scale;
}

const FVector3D& SceneComponent::AddWorldScale(float value)
{
    SetWorldScale(value + _World._scale._x, value + _World._scale._y, value);
    return _World._scale;
}

const FVector3D& SceneComponent::AddWorldScaleX(float x)
{
    SetWorldScale(x + _World._scale._x, _World._scale._y, _World._scale._z);
    return _World._scale;
}

const FVector3D& SceneComponent::AddWorldScaleY(float y)
{
    SetWorldScale(_World._scale._x, y + _World._scale._y, _World._scale._z);
    return _World._scale;
}

const FRotator& SceneComponent::GetWorldRotation() const
{
    return _World._rotation;
}

void SceneComponent::SetWorldRotation(const FVector2D& rot)
{
    SetWorldRotation(rot._x, rot._y, _World._rotation._z);
}

void SceneComponent::SetWorldRotation(float x, float y)
{
    SetWorldRotation(x, y, _World._rotation._z);
}

void SceneComponent::SetWorldRotation(const FRotator& rot)
{
    SetWorldRotation(rot._x, rot._y, rot._z);
}

void SceneComponent::SetWorldRotation(float x, float y, float z)
{
    if (Ptr<SceneComponent>parentComp = Lock<SceneComponent>(_Parent))
    {
        _Relative._rotation._x = x - parentComp->_World._rotation._x;
        _Relative._rotation._y = x - parentComp->_World._rotation._y;
        _Relative._rotation._z = x - parentComp->_World._rotation._z;
    }
    else
    {
        _Relative._rotation._x = x;
        _Relative._rotation._y = y;
        _Relative._rotation._z = z;
    }
    UpdateTransform();
}

const FTransform& SceneComponent::GetRelativeTransform() const
{
    return _Relative;
}

void SceneComponent::SetRelativeTransform(const FTransform& transform)
{
    SetRelativePosition(transform._position);
    SetRelativeScale(transform._scale);
    SetRelativeRotation(transform._rotation);
}

void SceneComponent::SetRelativeTransform(const FVector3D& pos, const FVector3D& scale, const FRotator& rot)
{
    SetRelativePosition(pos);
    SetRelativeScale(scale);
    SetRelativeRotation(rot);
}

const FVector3D& SceneComponent::GetRelativePosition() const
{
    return _Relative._position;
}

void SceneComponent::SetRelativePosition(const FVector2D& pos)
{
    SetRelativePosition(pos._x, pos._y, _Relative._position._z);
}

void SceneComponent::SetRelativePosition(float x, float y)
{
    SetRelativePosition(x, y, _Relative._position._z);
}

void SceneComponent::SetRelativePosition(const FVector3D& pos)
{
    SetRelativePosition(pos._x, pos._y, pos._z);
}

void SceneComponent::SetRelativePosition(float x, float y, float z)
{
    _Relative._position._x = x;
    _Relative._position._y = y;
    _Relative._position._z = z;
    UpdateTransform();
}

const FVector3D& SceneComponent::AddRelativePosition(const FVector2D& scale)
{
    SetRelativePosition(scale._x + _Relative._position._x, scale._y + _Relative._position._y, _Relative._position._z);
    return _Relative._position;
}

const FVector3D& SceneComponent::AddRelativePosition(float x, float y)
{
    SetRelativePosition(x + _Relative._position._x, y + _Relative._position._y, _Relative._position._z);
    return _Relative._position;
}

const FVector3D& SceneComponent::AddRelativePosition(const FVector3D& scale)
{
    SetRelativePosition(scale._x + _Relative._position._x, scale._y + _Relative._position._y, _Relative._position._z);
    return _Relative._position;
}

const FVector3D& SceneComponent::AddRelativePosition(float x, float y, float z)
{
    SetRelativePosition(x + _Relative._position._x , y + _Relative._position._y, z + _Relative._position._z);
    return _Relative._position;
}

const FVector3D& SceneComponent::AddRelativePosition(float value)
{
    SetRelativePosition(value + _Relative._position._x, value + _Relative._position._y, _Relative._position._z);
    return _Relative._position;
}

const FVector3D& SceneComponent::AddRelativePositionX(float x)
{
    SetRelativePosition(x + _Relative._position._x, _Relative._position._y, _Relative._position._z);
    return _Relative._position;
}

const FVector3D& SceneComponent::AddRelativePositionY(float y)
{
    SetRelativePosition(_Relative._position._x, y + _Relative._position._y, _Relative._position._z);
    return _Relative._position;
}

const FVector3D& SceneComponent::GetRelativeScale() const
{
    return _Relative._scale;
}

void SceneComponent::SetRelativeScale(const FVector2D& scale)
{
    SetRelativeScale(scale._x, scale._y, _Relative._scale._y);
}

void SceneComponent::SetRelativeScale(float x, float y)
{
    SetRelativeScale(x, y, _Relative._scale._y);
}

void SceneComponent::SetRelativeScale(const FVector3D& scale)
{
    SetRelativeScale(scale._x, scale._y, scale._z);
}

void SceneComponent::SetRelativeScale(float x, float y, float z)
{
    _Relative._scale._x = x;
    _Relative._scale._y = y;
    _Relative._scale._z = z;
    UpdateTransform();
}

const FVector3D& SceneComponent::AddRelativeScale(const FVector2D& scale)
{
    SetRelativeScale(scale._x + _Relative._scale._x, scale._y + _Relative._scale._y, _Relative._scale._z);
    return _Relative._scale;
}

const FVector3D& SceneComponent::AddRelativeScale(float x, float y)
{
    SetRelativeScale(x + _Relative._scale._x, y + _Relative._scale._y, _Relative._scale._z);
    return _Relative._scale;
}

const FVector3D& SceneComponent::AddRelativeScale(const FVector3D& scale)
{
    SetRelativeScale(scale._x + _Relative._scale._x, scale._y + _Relative._scale._y, scale._z + _Relative._scale._z);
    return _Relative._scale;
}

const FVector3D& SceneComponent::AddRelativeScale(float x, float y, float z)
{
    SetRelativeScale(x + _Relative._scale._x, y + _Relative._scale._y, z + _Relative._scale._z);
    return _Relative._scale;
}

const FVector3D& SceneComponent::AddRelativeScale(float value)
{
    SetRelativeScale(value + _Relative._scale._x, value + _Relative._scale._y, value + _Relative._scale._z);
    return _Relative._scale;
}

const FVector3D& SceneComponent::AddRelativeScaleX(float x)
{
    SetRelativeScale(x + _Relative._scale._x, _Relative._scale._y, _Relative._scale._z);
    return _Relative._scale;
}

const FVector3D& SceneComponent::AddRelativeScaleY(float y)
{
    SetRelativeScale(_Relative._scale._x, y + _Relative._scale._y, _Relative._scale._z);
    return _Relative._scale;
}

const FRotator& SceneComponent::GetRelativeRotation() const
{
    return _Relative._rotation;
}

void SceneComponent::SetRelativeRotation(const FVector2D& rot)
{
    SetRelativeRotation(rot._x, rot._y, _Relative._rotation._z);
}

void SceneComponent::SetRelativeRotation(float x, float y)
{
    SetRelativeRotation(x, y, _Relative._rotation._z);
}

void SceneComponent::SetRelativeRotation(const FRotator& rot)
{
    SetRelativeRotation(rot._x, rot._y, rot._z);
}

void SceneComponent::SetRelativeRotation(float x, float y, float z)
{
    _Relative._rotation._x = x;
    _Relative._rotation._y = y;
    _Relative._rotation._z = z;

    UpdateTransform();
}

const FRotator& SceneComponent::AddRelativeRotation(const FVector2D& rot)
{
    SetRelativeRotation(rot._x + _Relative._rotation._x, rot._y + _Relative._rotation._y, _Relative._rotation._z);
    return _Relative._rotation;
}

const FRotator& SceneComponent::AddRelativeRotation(float x, float y)
{
    SetRelativeRotation(x + _Relative._rotation._x, y + _Relative._rotation._y, _Relative._rotation._z);
    return _Relative._rotation;
}

const FRotator& SceneComponent::AddRelativeRotation(const FVector3D& rot)
{
    SetRelativeRotation(rot._x + _Relative._rotation._x, rot._y + _Relative._rotation._y, rot._z + _Relative._rotation._z);
    return _Relative._rotation;
}

const FRotator& SceneComponent::AddRelativeRotation(float x, float y, float z)
{
    SetRelativeRotation(x + _Relative._rotation._x, y + _Relative._rotation._y, z + _Relative._rotation._z);
    return _Relative._rotation;
}

const FRotator& SceneComponent::AddRelativeRotation(float value)
{
    SetRelativeRotation(value + _Relative._rotation._x, value + _Relative._rotation._y, value + _Relative._rotation._z);
    return _Relative._rotation;
}

const FRotator& SceneComponent::AddRelativeRotationX(float x)
{
    SetRelativeRotation(x + _Relative._rotation._x, _Relative._rotation._y, _Relative._rotation._z);
    return _Relative._rotation;
}

const FRotator& SceneComponent::AddRelativeRotationY(float y)
{
    SetRelativeRotation(_Relative._rotation._x, y + _Relative._rotation._y, _Relative._rotation._z);
    return _Relative._rotation;
}

void SceneComponent::UpdateTransform()
{
    _Matrix._scale.Scaling(_Relative._scale);
    _Matrix._rotation.Rotation(_Relative._rotation);
    _Matrix._translate.Translation(_Relative._position);
    FMatrix matLocal = _Matrix._scale * _Matrix._rotation * _Matrix._translate;
    if (Ptr<SceneComponent> parentComp = Lock<SceneComponent>(_Parent))
    {
        _Matrix._World = matLocal * parentComp->_Matrix._World;
    }
    else
    {
        _Matrix._World = matLocal;
    }
    
    _Matrix._World.ExtractPosition(_World._position);
    _Matrix._World.ExtractEuler(_World._rotation);
    _Matrix._World.ExtractScale(_World._scale);
    
    for (auto& it : _Childs)
    {
        if (it.second)
        {
            it.second->UpdateTransform();
        }
    }
    
    FVector3D axis[AXIS_TYPE::END] = 
    {
        FVector3D(FVector3D::Axis_X),
        FVector3D(FVector3D::Axis_Y),
        FVector3D(FVector3D::Axis_Z),
    };
    _Axis[AXIS_TYPE::X] = axis[AXIS_TYPE::X].TransformNormal(_Matrix._rotation);
    _Axis[AXIS_TYPE::Y] = axis[AXIS_TYPE::Y].TransformNormal(_Matrix._rotation);
    _Axis[AXIS_TYPE::Z] = axis[AXIS_TYPE::Z].TransformNormal(_Matrix._rotation);
}

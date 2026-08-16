#pragma once
#include "Component.h"
#include <map>
class SceneComponent : public Component
{
    friend class Actor;
public:
    SceneComponent();
    virtual~SceneComponent();
    
protected:
    Ptr<class TransformCBuffer> _TransformCBuffer;
    Weak<SceneComponent> _Parent;
    std::map<int32, Ptr<SceneComponent>> _Childs;
    std::string _RenderLayerName;
    
    FTransform       _World;
    FTransform       _Relative;
    FTransformMatrix _Matrix;

    FVector3D _Axis[AXIS_TYPE::END] =
    {
        FVector3D(FVector3D::Axis_X),
        FVector3D(FVector3D::Axis_Y),
        FVector3D(FVector3D::Axis_Z)
    };
    
    bool _IsRender = false;
    
public:
    virtual bool Init(int32 Id, const std::string& Name, Ptr<class Actor> Owner) override;
    virtual void Tick(float DeltaTime) override;
    virtual void Collision(float DeltaTime) override;
    virtual void Render(float DeltaTime) override;
    virtual void Destroy() override;
    virtual void Save(std::ofstream& file);
    virtual void Load(std::ifstream& file);
    
public:
    void SetRenderLayerName(const std::string& name);
    const std::string& GetRenderLayerName() const;
    const std::map<int32, Ptr<SceneComponent>>& GetChilds() const;
    const Ptr<SceneComponent>& GetParent() const;
    void AttachToComponent(Ptr<SceneComponent> comp);
    void AddChild(Ptr<SceneComponent> comp);
    Ptr<SceneComponent> FindComponent(int32 id) const;
    
public:;
    
    // World Transform
    const FTransform& GetWorldTransform() const;
    void SetWorldTransform(const FTransform& transform);
    void SetWorldTransform(const FVector3D& pos, const FVector3D& scale, const FRotator& rot);
    
    // World Position
    const FVector3D& GetWorldPosition() const;
    void SetWorldPosition(const FVector2D& pos);
    void SetWorldPosition(float x, float y);
    void SetWorldPosition(const FVector3D& pos);
    void SetWorldPosition(float x, float y, float z);
    const FVector3D& AddWorldPosition(const FVector2D& pos);
    const FVector3D& AddWorldPosition(float x, float y);
    const FVector3D& AddWorldPosition(const FVector3D& pos);
    const FVector3D& AddWorldPosition(float x, float y, float z);
    const FVector3D& AddWorldPosition(float value);
    const FVector3D& AddWorldPositionX(float x);
    const FVector3D& AddWorldPositionY(float y);
    
    // World Scale
    const FVector3D& GetWorldScale() const;
    void SetWorldScale(const FVector2D& scale);
    void SetWorldScale(float x, float y);
    void SetWorldScale(const FVector3D& scale);
    void SetWorldScale(float x, float y, float z);
    const FVector3D& AddWorldScale(const FVector2D& scale);
    const FVector3D& AddWorldScale(float x, float y);
    const FVector3D& AddWorldScale(const FVector3D& scale);
    const FVector3D& AddWorldScale(float x, float y, float z);
    const FVector3D& AddWorldScale(float value);
    const FVector3D& AddWorldScaleX(float x);
    const FVector3D& AddWorldScaleY(float y);
    
    // World Rotation
    const FRotator& GetWorldRotation() const;
    void SetWorldRotation(const FVector2D& rot);
    void SetWorldRotation(float x, float y);
    void SetWorldRotation(const FRotator& rot);
    void SetWorldRotation(float x, float y, float z);
    
    // Relative Transform
    const FTransform& GetRelativeTransform() const;
    void SetRelativeTransform(const FTransform& transform);
    void SetRelativeTransform(const FVector3D& pos, const FVector3D& scale, const FRotator& rot);

    // Relative Position
    const FVector3D& GetRelativePosition() const;
    void SetRelativePosition(const FVector2D& pos);
    void SetRelativePosition(float x, float y);
    void SetRelativePosition(const FVector3D& pos);
    void SetRelativePosition(float x, float y, float z);
    const FVector3D& AddRelativePosition(const FVector2D& scale);
    const FVector3D& AddRelativePosition(float x, float y);
    const FVector3D& AddRelativePosition(const FVector3D& scale);
    const FVector3D& AddRelativePosition(float x, float y, float z);
    const FVector3D& AddRelativePosition(float value);
    const FVector3D& AddRelativePositionX(float x);
    const FVector3D& AddRelativePositionY(float y);
    
    // Relative Scale 
    const FVector3D& GetRelativeScale() const;
    void SetRelativeScale(const FVector2D& scale);
    void SetRelativeScale(float x, float y);
    void SetRelativeScale(const FVector3D& scale);
    void SetRelativeScale(float x, float y, float z);
    const FVector3D& AddRelativeScale(const FVector2D& scale);
    const FVector3D& AddRelativeScale(float x, float y);
    const FVector3D& AddRelativeScale(const FVector3D& scale);
    const FVector3D& AddRelativeScale(float x, float y, float z);
    const FVector3D& AddRelativeScale(float value);
    const FVector3D& AddRelativeScaleX(float x);
    const FVector3D& AddRelativeScaleY(float y);
    
    // Relative Rotation 
    const FRotator& GetRelativeRotation() const;
    void SetRelativeRotation(const FVector2D& rot);
    void SetRelativeRotation(float x, float y);
    void SetRelativeRotation(const FRotator& rot);
    void SetRelativeRotation(float x, float y, float z);
    const FRotator& AddRelativeRotation(const FVector2D& rot);
    const FRotator& AddRelativeRotation(float x, float y);
    const FRotator& AddRelativeRotation(const FVector3D& rot);
    const FRotator& AddRelativeRotation(float x, float y, float z);
    const FRotator& AddRelativeRotation(float value);
    const FRotator& AddRelativeRotationX(float x);
    const FRotator& AddRelativeRotationY(float y);
    
private:
    void UpdateTransform();
};

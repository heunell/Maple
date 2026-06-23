#pragma once
#include "Component.h"
class SceneComponent : public Component
{
public:
    SceneComponent();
    ~SceneComponent();
    SceneComponent(const SceneComponent&) = delete;
    SceneComponent(SceneComponent&&) = delete;
    SceneComponent& operator=(const SceneComponent&) = delete;
    SceneComponent& operator=(SceneComponent&&) = delete;
    
private:
    // Rider Git 초기화 과정 테스트 
};

#pragma once
#include "UI.h"

class LucidEntryDialogUI : public UI
{
public:
    LucidEntryDialogUI() = default;
    virtual ~LucidEntryDialogUI() = default;
    
private:
    Weak<class LucidEntryNPC> _NPC;
    
    // NPC를 클릭한 입력이 UI 버튼까지 전달되는걸 막기
    bool _WaitRelease = true;
    
public:
    bool Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator,  const std::string& Name, Ptr<class LucidEntryNPC> NPC);
    
    virtual void Tick(float DeltaTime) override;
    
    virtual void Destroy() override;
    
    void Open();
    
    void Close();
    
private:
    void Confirm();
    
};

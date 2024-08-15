#pragma once
#include "EditorUI.h"

#include "FlipBookEditor.h"
#include <Engine/CAssetMgr.h>

class FE_Sub :
    public EditorUI
{
private:
    FlipBookEditor* m_Owner;

public:
    FlipBookEditor* GetOwner() { return m_Owner; }
    class FE_FBViewer* GetViewer() { return m_Owner->GetFBViewer(); }
    class FE_FBDetail* GetDetail() { return m_Owner->GetDetail(); }
    class FE_SpriteList* GetSpriteList() { return m_Owner->GetSpriteList(); }
public:
    FE_Sub();
    ~FE_Sub();

    friend class FlipBookEditor;
};

